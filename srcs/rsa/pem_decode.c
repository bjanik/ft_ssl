#include "bn.h"
#include "ft_ssl.h"
#include "assert.h"

/*
    Take a fd as parameter, reads its content and parses to get all components of rsa_data:
    First need to decode base64
    --> modulus
    --> public exp
    --> private exp
    --> prime1
    --> prime2
    --> exponent 1
    --> exponent 2
    --> coefficient
    check_flag: 

*/

uint32_t get_number_len(unsigned char **ptr)
{
    uint32_t len;

    len = 0;
    if (**ptr == 0x81)
        (*ptr)++;
    else if (**ptr == 0x82)
    {
        (*ptr)++;
        len = (uint32_t)**ptr << 8;
        (*ptr)++;
    }
    len |= **ptr;
    (*ptr)++;
    return (len);
}

t_bn     *retrieve_number_from_data(unsigned char **data)
{
    t_bn        *n;
    uint8_t     bytes_in_limb;
    uint32_t    len; 

    (*data)++;
    len = get_number_len(data);
    if ((n = bn_init_size(len * 8)) == NULL)
        return (NULL);
    n->size = len / 8;
    if (len % 8)
        INC_SIZE(n);
    while (len)
    {
        bytes_in_limb = len % 8;
        if (len % 8 == 0)
            bytes_in_limb = 8;
        len -= bytes_in_limb;
        while (bytes_in_limb)
        {
            n->num[len / 8] |= (uint64_t)**data << (--bytes_in_limb * 8);
            (*data)++;
        }
    }
    return (n);
}

int      parse_decoded_data(t_rsa *rsa,
                                   unsigned char *decoded_data,
                                   uint32_t decoded_data_len)
{
    unsigned char *ptr = decoded_data;
    uint32_t      cur_len = 0;

    if (*ptr++ != 0x30)
        return (1);
    cur_len = get_number_len(&ptr);
    if (cur_len + ptr > decoded_data + decoded_data_len)
        return (1);
    ptr += 3;
    rsa->rsa_data.modulus = retrieve_number_from_data(&ptr);
    rsa->rsa_data.public_exp = retrieve_number_from_data(&ptr);
    if ((rsa->opts & RSA_PUBIN) == 0 || (rsa->opts & RSA_PUBOUT))
    {
        rsa->rsa_data.private_exp = retrieve_number_from_data(&ptr);
        rsa->rsa_data.prime1 = retrieve_number_from_data(&ptr);
        rsa->rsa_data.prime2 = retrieve_number_from_data(&ptr);
        rsa->rsa_data.exponent1 = retrieve_number_from_data(&ptr);
        rsa->rsa_data.exponent2 = retrieve_number_from_data(&ptr);
        rsa->rsa_data.coef = retrieve_number_from_data(&ptr);
    }
    return (0);
}

char            *get_pem_passphrase(const char *in, int decryption)
{
    char    *pwd;
    char    *check_pwd;
    char    *prompt;

    if (decryption)
        prompt = ft_strnjoin("Enter pass phrase for ", 2, in, ":");
    else
        prompt = ft_strdup("Enter PEM pass phrase:");
    if (prompt == NULL)
        return (NULL);
    pwd = ft_strdup(getpass(prompt));
    if (decryption && pwd)
        return (pwd);
    if (ft_strlen(pwd) < 4)
    {
        ft_putendl_fd("ft_ssl: password too small, need at least 4 bytes", STDERR_FILENO);
        ft_strdel(&pwd);
        ft_strdel(&prompt);
        return (NULL);
    }
    if (decryption == 0)
        check_pwd = getpass("Verifying - Enter PEM pass phrase:");
    if (pwd == NULL || check_pwd == NULL)
        ft_putendl_fd("ft_ssl: bad PEM password read", STDERR_FILENO);
    else if (ft_strcmp(check_pwd, pwd) != 0)
    {
        ft_putendl_fd("ft_ssl: Verify password failure", STDERR_FILENO);
        ft_strdel(&pwd);
    }
    return (pwd);
}

static int          key_from_passphrase(t_rsa *rsa, int decryption)
{
    unsigned char   *hash;
    unsigned char   keys[4][8];
    int             nb;

    if ((rsa->des->password = get_pem_passphrase(rsa->in, decryption)) == NULL)
        return (1);
    if (rsa->des->init_vector == 0)
    {
        if (get_salt(rsa->des))
            return (1);
    }
    else
    {
        rsa->des->salt = (unsigned char *)malloc(8 * sizeof(unsigned char));
        cipher_to_string(rsa->des->init_vector, rsa->des->salt);
    }
    nb = (ft_strchr(rsa->des->name, '3')) ? TRIPLE_DES : SINGLE_DES;
    if (!(hash = pbkdf(rsa->des->password, rsa->des->salt, nb)))
        return (1);
    set_key(rsa->des, hash, keys, nb);
    ft_memdel((void**)&hash);
    rsa->des->init_vector = convert_input_to_block(rsa->des->salt);
    return (0);
}

unsigned char       *private_key_decryption(t_rsa *rsa,
                                            unsigned char *data,
                                            uint32_t *data_len)
{
    unsigned char   *decrypted_data;
    unsigned char   *decrypted_data_no_pad;

    if (key_from_passphrase(rsa, 1))
        return (NULL);
    swap_keys(rsa->des->keys[0]);
    swap_keys(rsa->des->keys[1]);
    swap_keys(rsa->des->keys[2]);
    decrypted_data = des_decrypt_data(rsa->des, data, *data_len);
    if (decrypted_data[*data_len - 1] > 8)
    {
        ft_putendl_fd("ft_ssl: Bad decrypt", STDERR_FILENO);
        return (NULL);
    }
    if (ft_memcmp(decrypted_data + *data_len - decrypted_data[*data_len - 1],
                  g_padding_patterns[decrypted_data[*data_len - 1] - 1],
                  decrypted_data[*data_len - 1]))
        return (NULL);
    *data_len = *data_len - decrypted_data[*data_len - 1];
    if ((decrypted_data_no_pad = (unsigned char*)malloc(*data_len)) == NULL)
        return (NULL);
    ft_memcpy(decrypted_data_no_pad, decrypted_data, *data_len);
    ft_memdel((void**)&decrypted_data);
    ft_memdel((void**)&data);
    return (decrypted_data_no_pad);
}

unsigned char           *private_key_encryption(t_rsa *rsa,
                                                unsigned char *data,
                                                uint32_t *data_len)
{
    unsigned char   *des_encrypted_data;
    
    if (key_from_passphrase(rsa, 0))
        return (NULL);
    des_encrypted_data = des_encrypt_data(rsa->des, data, data_len);
    return (des_encrypted_data);
}