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
    if ((decrypted_data_no_pad = (unsigned char*)ft_malloc(*data_len)) == NULL)
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