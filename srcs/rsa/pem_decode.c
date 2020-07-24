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

t_bn    *get_bn(unsigned char **data, uint32_t len)
{
    t_bn        *n;
    uint8_t     bytes_in_limb;

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

t_bn     *retrieve_number_from_data(unsigned char **data)
{
    t_bn        *n;
    uint32_t    len;

    (*data)++;
    len = get_number_len(data);
    if (**data == 0)
    {
        (*data)++;
        len--;
    }
    n = get_bn(data, len);
    return (n);
}

int      parse_decoded_data(t_rsa_data *rsa_data,
                            unsigned char *decoded_data,
                            uint32_t decoded_data_len,
                            int opts)
{
    unsigned char *ptr = decoded_data;
    uint32_t      cur_len = 0;

    if (*ptr++ != 0x30)
        return (1);
    cur_len = get_number_len(&ptr);
    if (cur_len + ptr > decoded_data + decoded_data_len)
        return (1);
    ptr += 3;
    rsa_data->modulus = retrieve_number_from_data(&ptr);
    rsa_data->public_exp = retrieve_number_from_data(&ptr);
    if ((opts & RSA_PUBIN) == 0)
    {
        rsa_data->private_exp = retrieve_number_from_data(&ptr);
        rsa_data->prime1 = retrieve_number_from_data(&ptr);
        rsa_data->prime2 = retrieve_number_from_data(&ptr);
        rsa_data->exponent1 = retrieve_number_from_data(&ptr);
        rsa_data->exponent2 = retrieve_number_from_data(&ptr);
        rsa_data->coef = retrieve_number_from_data(&ptr);
    }
    return (0);
}

char            *get_pem_passphrase(const char *in, int decryption)
{
    char    *pwd = NULL;
    char    *check_pwd = NULL;
    char    *prompt;

    if (decryption)
        prompt = ft_strnjoin("Enter pass phrase for ", 2, in, ":");
    else
        prompt = ft_strdup("Enter PEM pass phrase:");
    if (prompt == NULL)
        return (NULL);
    pwd = ft_strdup(getpass(prompt));
    if (decryption || pwd == NULL)
        return (pwd);
    if (ft_strlen(pwd) < 4)
    {
        ft_dprintf(STDERR_FILENO, "ft_ssl: password too small, need at least 4 bytes\n");
        ft_strdel(&pwd);
        ft_strdel(&prompt);
        return (NULL);
    }
    if (decryption == 0)
        check_pwd = getpass("Verifying - Enter PEM pass phrase:");
    if (pwd == NULL || check_pwd == NULL)
        ft_dprintf(STDERR_FILENO, "ft_ssl: bad PEM password read\n");
    else if (ft_strcmp(check_pwd, pwd) != 0)
        ft_dprintf(STDERR_FILENO, "ft_ssl: Verify password failure\n");
    ft_strdel(&check_pwd);
    return (pwd);
}

static int          key_from_passphrase(t_des *des, const int decryption, const char *in, const char *pass)
{
    unsigned char   *hash;
    unsigned char   keys[4][8];

    if (pass == NULL)
    {
        if ((des->password = get_pem_passphrase(in, decryption)) == NULL)
            return (1);
    }
    else
        des->password = ft_strdup(pass);
    if (des->init_vector == 0)
    {
        if (get_salt(des))
            return (1);
    }
    else
    {
        des->salt = (unsigned char *)malloc(8 * sizeof(unsigned char));
        cipher_to_string(des->init_vector, des->salt);
    }
    if (!(hash = pbkdf(des->password, des->salt, SINGLE_DES)))
        return (1);
    set_key(des, hash, keys, SINGLE_DES);
    ft_memdel((void**)&hash);
    des->init_vector = convert_input_to_block(des->salt);
    return (0);
}

unsigned char       *private_key_decryption(t_des *des,
                                            unsigned char *data,
                                            uint32_t *data_len,
                                            const char *in,
                                            const char *pass)
{
    unsigned char   *decrypted_data;
    unsigned char   *decrypted_data_no_pad;

    if (key_from_passphrase(des, 1, in, pass))
        return (NULL);
    swap_keys(des->keys[0]);
    swap_keys(des->keys[1]);
    swap_keys(des->keys[2]);
    decrypted_data = des_decrypt_data(des, data, *data_len);
    if (decrypted_data[*data_len - 1] > 8)
    {
        ft_dprintf(STDERR_FILENO, "ft_ssl: Bad decrypt\n");
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

unsigned char           *private_key_encryption(t_des *des,
                                                unsigned char *data,
                                                uint32_t *data_len,
                                                const char *in, const char *pass)
{
    unsigned char   *des_encrypted_data;
    
    if (key_from_passphrase(des, 0, in, pass))
        return (NULL);
    des_encrypted_data = des_encrypt_data(des, data, data_len);
    ft_memdel((void**)&data);
    return (des_encrypted_data);
}