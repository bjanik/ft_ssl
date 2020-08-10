/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 15:39:21 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 15:39:22 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "bn.h"

static uint32_t	get_bn_len_encoding(t_bn *n)
{
	uint32_t	len;

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
	len++;
	return (len);
}

uint32_t		get_pem_data_len(t_rsa_data *rsa_data)
{
	uint32_t	data_len;

	data_len = 3;
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
	data_len++;
	return (data_len);
}

void			set_len_to_data(uint32_t n, unsigned char *data, uint32_t *len)
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

void			write_bn_to_data(t_bn *n, unsigned char *data)
{
	uint32_t	bytes;
	int			i;
	int			j;

	bytes = get_byte_number(n->num[SIZE(n) - 1]);
	i = n->size;
	while (--i >= 0)
	{
		j = bytes;
		while (--j >= 0)
			*data++ = (n->num[i] >> (j * 8)) & 0xFF;
		bytes = 8;
	}
}

void			set_bn_to_data(t_bn *n, unsigned char *data, uint32_t *len)
{
	uint32_t	bytes_in_len;
	uint32_t	len_bn;

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
