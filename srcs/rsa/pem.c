/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pem.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:55:04 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:55:05 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"
#include "ft_ssl.h"

static void		set_version_to_data(unsigned char *data, uint32_t *len)
{
	data[(*len)++] = 0x02;
	data[(*len)++] = 0x01;
	data[(*len)++] = 0x00;
}

static void		fill_pem_private_data(t_rsa_data *rsa, unsigned char *data,
										uint32_t data_len)
{
	uint32_t	len;
	uint32_t	bytes;

	len = 0;
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

int				pem_output(char *data_encoded, int fd)
{
	uint32_t		offset;
	size_t			data_encoded_len;

	offset = 0;
	data_encoded_len = ft_strlen(data_encoded);
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

int				pem(t_rsa_data *rsa, int fd)
{
	uint32_t		data_len;
	unsigned char	*data;
	char			*data_encoded;

	data_len = get_pem_data_len(rsa);
	if ((data = (unsigned char*)ft_malloc(data_len + 3)) == NULL)
		return (1);
	fill_pem_private_data(rsa, data, data_len);
	if ((data_encoded = base64_encode_data(data, data_len)) == NULL)
		return (1);
	pem_output(data_encoded, fd);
	ft_memdel((void**)&data_encoded);
	ft_memdel((void**)&data);
	return (0);
}
