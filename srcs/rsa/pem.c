#include "bn.h"
#include "ft_ssl.h"

void	set_len_to_data(uint32_t n, unsigned char *data, uint32_t *len)
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

void 			write_bn_to_data(t_bn *n, unsigned char *data)
{
	uint32_t 	bytes;
	int			i, j;

	bytes = get_byte_number(n->num[SIZE(n) - 1]);
	for (i = SIZE(n) - 1 ; i >= 0; i--)
	{
		for (j = bytes - 1; j >= 0; j--, data++)
			*data = (n->num[i] >> (j * 8)) & 0xFF;
		bytes = 8;
	}
}

void		set_bn_to_data(t_bn *n, unsigned char *data, uint32_t *len)
{
	uint32_t	bytes_in_len, len_bn;

	data[(*len)++] = 0x02;
	len_bn = bn_len(n);
	bytes_in_len = get_byte_number(len_bn);
	if (len_bn >= 0x80)
		data[(*len)++] = 0x80 + bytes_in_len;
	set_len_to_data(len_bn + (bn_get_bit_number(n) % 8 == 0), data, len);
	if (bn_get_bit_number(n) % 8 == 0)
		data[(*len)++] = 0;
	write_bn_to_data(n, data + *len);
	*len += len_bn;
}

static void	set_version_to_data(unsigned char *data, uint32_t *len)
{
	data[(*len)++] = 0x02;
	data[(*len)++] = 0x01;
	data[(*len)++] = 0x00;
}

static uint32_t 	get_bn_len_encoding(t_bn *n)
{
	uint32_t len;

	len = bn_len(n);
	if (bn_get_bit_number(n) % 8 == 0)
		len++;
	if (len < 0x80)
		len++;
	else if (len <= 0xFF)
		len += 2;
	else if (len <= 0xFFFF)
		len += 3;
	else if (len <= 0xFFFFFF)
		len += 4;
	len++; // 0x02 byte value;
	return (len);
}

uint32_t	get_pem_data_len(t_rsa_data *rsa_data)
{
	uint32_t	data_len;

	data_len = 3; // bytes sequences 0x2 0x1 0x0
	data_len += get_bn_len_encoding(rsa_data->modulus);
	data_len += get_bn_len_encoding(rsa_data->public_exp);
	data_len += get_bn_len_encoding(rsa_data->private_exp);
	data_len += get_bn_len_encoding(rsa_data->prime1);
	data_len += get_bn_len_encoding(rsa_data->prime2);
	data_len += get_bn_len_encoding(rsa_data->exponent1);
	data_len += get_bn_len_encoding(rsa_data->exponent2);
	data_len += get_bn_len_encoding(rsa_data->coef);
	if (data_len < 0x80)
		data_len++;
	else if (data_len <= 0xFF)
		data_len += 2;
	else if (data_len <= 0xFFFF)
		data_len += 3;
	else if (data_len <= 0xFFFFFF)
		data_len += 4;
	data_len++; // First byte 0x30
	return (data_len);
}

static void	fill_pem_private_data(t_rsa_data *rsa, unsigned char *data, uint32_t data_len)
{
	uint32_t 	len = 0;
	uint32_t 	bytes;

	data[len++] = 0x30;
	data_len--;
	if (data_len-- >= 0x80)
	{
		bytes = get_byte_number(data_len);
		data[len++] = 0x80 + bytes;
		data_len -= bytes;
	}
	set_len_to_data(data_len, data, &len);
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

int 		pem_output(char *data_encoded, int fd)
{
	uint32_t 		offset = 0;
	size_t 			data_encoded_len = ft_strlen(data_encoded);

	ft_putendl_fd(PEM_PRIVATE_HEADER, fd);
	while (data_encoded_len > 64)
	{
		write(fd, data_encoded + offset, 64);
		write(fd, "\n", 1);
		offset += 64;
		data_encoded_len -= 64;
	}
	write(fd, data_encoded + offset, data_encoded_len);
	write(fd, "\n", 1);
	ft_putendl_fd(PEM_PRIVATE_FOOTER, fd);
	return (0);
}

int			pem(t_rsa_data *rsa, int fd)
{
	uint32_t 		data_len;
	unsigned char 	*data;
	char 			*data_encoded = NULL;
	
	data_len = get_pem_data_len(rsa);
	if ((data = (unsigned char*)malloc(data_len + 3)) == NULL)
		return (1);
	fill_pem_private_data(rsa, data, data_len);
	if ((data_encoded = base64_encode_data(data, data_len)) == NULL)
		return (1);
	pem_output(data_encoded, fd);
	ft_memdel((void**)&data_encoded);
	ft_memdel((void**)&data);
	return (0);
}