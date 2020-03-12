#include "bn.h"
#include "ft_ssl.h"

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

static unsigned char    *pem_get_decoded_data(uint32_t *decoded_data_len, 
                                              unsigned char *data, uint32_t data_len)
{
    uint32_t        offset;
    unsigned char   *decoded_data;

    if ((decoded_data = (unsigned char *)malloc(data_len * sizeof(char)))== NULL)
        return (NULL);
    offset = 0;
    while (offset < data_len)
    {
        decode((unsigned char*)data + offset, decoded_data + *decoded_data_len, 4);
        *decoded_data_len += 3;
        offset += 4;
    }
    return (decoded_data);
}

static uint32_t get_number_len(unsigned char **ptr)
{
    uint32_t len;

    printf("**ptr = %hhX\n", **ptr);

    if (**ptr > 0x80)
        (*ptr)++;
    len = (uint32_t)**ptr;
    return (len);
}

static t_bn     *retrieve_number_from_data(unsigned char **data)
{
    t_bn        *n;
    uint8_t     bytes_in_limb;
    uint32_t    len = get_number_len(data);

    // printf("len = %u\n", len);
    (*data)++;
    // printf("**data = %hhX\n", **data);
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


static int      parse_decoded_data(t_rsa *rsa, unsigned char *decoded_data, uint32_t decoded_data_len)
{
    unsigned char *ptr = decoded_data;
    uint32_t      cur_len, byte_len, len;

    if (*ptr++ != 0x30)
        return (1);
    len = *ptr++ - 0x80;
    if (len > 1)
    {
        cur_len = (uint32_t)*ptr++ << 8;
        cur_len |= *ptr++;
    }   
    else
        cur_len = (uint32_t)*ptr++;
    // if (cur_len + len + 1 != decoded_data_len)
    //     return (1);
    ptr += 3;
    ptr += 1;
    rsa->rsa_data.modulus = retrieve_number_from_data(&ptr);
    ptr += 1;
    rsa->rsa_data.public_exp = retrieve_number_from_data(&ptr);
    ptr += 1;
    rsa->rsa_data.private_exp = retrieve_number_from_data(&ptr);
    ptr += 1;
    rsa->rsa_data.prime1 = retrieve_number_from_data(&ptr);
    ptr += 1;
    rsa->rsa_data.prime2 = retrieve_number_from_data(&ptr);
    ptr += 1;
    rsa->rsa_data.exponent1 = retrieve_number_from_data(&ptr);
    ptr += 1;
    rsa->rsa_data.exponent2 = retrieve_number_from_data(&ptr);
    ptr += 1;
    rsa->rsa_data.coef = retrieve_number_from_data(&ptr);
    display_bn_as_text(rsa->rsa_data.modulus);
    return (0);

}

int             pem_decode(t_rsa *rsa, char *data)
{
    uint32_t        data_len, decoded_data_len, offset;
    unsigned char   *decoded_data;
    int ret;

    data_len = ft_strlen(data);
    decoded_data_len = 0;
    if (data_len % 4)
        return (1); // BASE64 ERROR
    decoded_data = pem_get_decoded_data(&decoded_data_len, (unsigned char*)data, data_len);
    if (decoded_data == NULL)
        return (1);
    ret = parse_decoded_data(rsa, decoded_data, decoded_data_len);
    // write(rsa->fd[OUT], decoded_data, decoded_data_len);
    return (ret);
}