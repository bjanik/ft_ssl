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

static void		print_private_key(t_genrsa *genrsa, char *data, const int fd)
{
	int	i;
	int	len;

	len = ft_strlen(data);
	i = 0;
	ft_dprintf(STDERR_FILENO, "writing RSA key\n");
	ft_dprintf(fd, "%s\n", PEM_PRIVATE_HEADER);
	if (genrsa->opts & GENRSA_DES)
		print_encryption_header(genrsa->des, fd);
	while (len > 0)
	{
		if (len > 64)
			write(fd, data + i, 64);
		else
			write(fd, data + i, len);
		i += 64;
		len -= 64;
		write(fd, "\n", 1);
	}
	ft_dprintf(fd, "%s\n", PEM_PRIVATE_FOOTER);
}

int				pem(t_genrsa *genrsa)
{
	uint32_t		data_len;
	unsigned char	*data;
	char			*data_encoded;
	char			*args[2];

	args[0] = NULL;
	args[1] = genrsa->passout;
	data_len = get_pem_data_len(&genrsa->rsa_data);
	if ((data = (unsigned char*)ft_malloc(data_len + 3)) == NULL)
		return (1);
	fill_pem_private_data(&genrsa->rsa_data, data, data_len);
	if (genrsa->opts & GENRSA_DES)
		data = private_key_encryption(genrsa->des, data, &data_len, args);
	if (data == NULL)
		return (1);
	if ((data_encoded = base64_encode_data(data, data_len)) == NULL)
		return (1);
	print_private_key(genrsa, data_encoded, genrsa->fd[OUT]);
	ft_memdel((void**)&data_encoded);
	ft_memdel((void**)&data);
	return (0);
}
