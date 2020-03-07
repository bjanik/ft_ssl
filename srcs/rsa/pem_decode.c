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

static int      parse_decoded_data(t_rsa *rsa, unsigned char *decoded_data)
{
    unsigned char *ptr = decoded_data;

    
}

int             pem_decode(t_rsa *rsa, char *data)
{
    uint32_t        data_len, decoded_data_len, offset;
    unsigned char   *decoded_data;

    data_len = ft_strlen(data);
    decoded_data_len = 0;
    if (data_len % 4)
        return (1); // BASE64 ERROR
    if ((decoded_data = pem_get_decoded_data(&decoded_data_len, (unsigned char*)data, data_len)) == NULL)
        return (1);
    parse_decoded_data(rsa, decoded_data);
    // write(rsa->fd[OUT], decoded_data, decoded_data_len);
    return (0);
}