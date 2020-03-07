#include "bn.h"
#include "ft_ssl.h"

// Strcuture : magic 0x30 0x82
//			   data len (total len - len(magic) - len(data_len))
//			   version (0x02 0x01 0x00)
//			   len(modulus) +  modulus


static void	set_len_to_data(uint32_t n, unsigned char *data, uint32_t *len)
{
	if (n <= 0xFF)
		data[(*len)++] = n;
	else if (n <= 0xFFFF)
	{
		data[(*len)++] = n >> 8 & 0xFF;
		data[(*len)++] = n & 0xFF;
	}
	else if (n <= 0xFFFFFF)
	{
		data[(*len)++] = n >> 16 & 0xFF;
		data[(*len)++] = n >> 8 & 0xFF;
		data[(*len)++] = n & 0xFF;
	}
	else if (n <= 0xFFFFFFFF)
	{
		data[(*len)++] = n >> 24 & 0xFF;
		data[(*len)++] = n >> 16 & 0xFF;
		data[(*len)++] = n >> 8 & 0xFF;
		data[(*len)++] = n & 0xFF;
	}
}

static void	set_bn_to_data(t_bn *n, unsigned char *data, uint32_t *len)
{
	uint32_t 	bytes;
	int			i, j;
	uint32_t	bytes_in_len, bn_len;

	data[(*len)++] = 0x02;
	bn_len = bn_get_byte_number(n);
	bytes_in_len = get_byte_number(bn_len);
	if (bn_len >= 0x80)
		data[(*len)++] = 0x80 + bytes_in_len;
	set_len_to_data(bn_len, data, len);
	bytes = get_byte_number(n->num[SIZE(n) - 1]);
	for (i = SIZE(n) - 1 ; i >= 0; i--)
	{
		for (j = bytes - 1; j >= 0; j--)
			data[(*len)++] = (n->num[i] >> (j * 8)) & 0xFF;
		bytes = 8;
	}
}

static void	set_version_to_data(unsigned char *data, uint32_t *len)
{
	data[(*len)++] = 0x02;
	data[(*len)++] = 0x01;
	data[(*len)++] = 0x00;
}

uint32_t	get_pem_data_len(t_rsa_data *rsa)
{
	uint32_t	data_len;

	data_len = 18;
	data_len += bn_get_byte_number(rsa->modulus);
	data_len += bn_get_byte_number(rsa->public_exp);
	data_len += bn_get_byte_number(rsa->private_exp);
	data_len += bn_get_byte_number(rsa->prime1);
	data_len += bn_get_byte_number(rsa->prime2);
	data_len += bn_get_byte_number(rsa->exponent1);
	data_len += bn_get_byte_number(rsa->exponent2);
	data_len += bn_get_byte_number(rsa->coef);
	return (data_len);
}

static void	fill_pem_data(t_rsa_data *rsa, unsigned char *data, uint32_t data_len)
{
	uint32_t 	len = 0;

	data[len++] = 0x30;
	data[len++] = 0x80 + get_byte_number(data_len);
	set_len_to_data(data_len - 2 - get_byte_number(data_len), data, &len);
	set_version_to_data(data, &len);
	set_bn_to_data(rsa->modulus, data, &len);
	set_bn_to_data(rsa->public_exp, data, &len);
	set_bn_to_data(rsa->private_exp, data, &len);
	set_bn_to_data(rsa->prime1, data, &len);
	set_bn_to_data(rsa->prime2, data, &len);
	set_bn_to_data(rsa->exponent1, data, &len);
	set_bn_to_data(rsa->exponent2, data, &len);
	set_bn_to_data(rsa->coef, data, &len);
}

static int pem_output(unsigned char *data, uint32_t data_len, int fd)
{
	uint32_t 		offset, data_encoded_len;
	unsigned char	*data_encoded;

	if ((data_encoded = (unsigned char*)malloc(data_len * 2)) == NULL)
		return (1);
	offset = 0;
	data_encoded_len = 0;
	ft_putendl_fd(PEM_HEADER, fd);
	while (offset < data_len)
	{
		encode(data + offset, data_encoded + data_encoded_len, data_len - offset);
		offset += 3;
		data_encoded_len += 4;
	}
	offset = 0;
	while (data_encoded_len > 64)
	{
		write(fd, data_encoded + offset, 64);
		write(fd, "\n", 1);
		offset += 64;
		data_encoded_len -= 64;
	}
	write(fd, data_encoded + offset, data_encoded_len);
	write(fd, "\n", 1);
	ft_putendl_fd(PEM_FOOTER, fd);
	ft_memdel((void**)&data_encoded);
	return (0);
}

int			pem(t_rsa_data *rsa, int fd)
{
	uint32_t 		data_len;
	unsigned char 	*data;
	

	data_len = get_pem_data_len(rsa);
	if ((data = (unsigned char*)malloc(data_len + 1)) == NULL)
		return (1);
	fill_pem_data(rsa, data, data_len);
	pem_output(data, data_len, fd);
	return (0);
}