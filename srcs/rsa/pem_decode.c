/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pem_decode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:32:52 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:32:55 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"
#include "ft_ssl.h"

uint32_t	get_number_len(unsigned char **ptr)
{
	uint32_t	len;

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

t_bn		*get_bn(unsigned char **data, uint32_t len)
{
	t_bn		*n;
	uint8_t		bytes_in_limb;

	if ((n = bn_init_size(len * 8)) == NULL)
		return (NULL);
	n->size = len / 8;
	if (len % 8)
		n->size++;
	while (len)
	{
		bytes_in_limb = len % 8;
		if (len % 8 == 0)
			bytes_in_limb = 8;
		len -= bytes_in_limb;
		while (bytes_in_limb)
		{
			n->num[len / 8] |= (uint64_t)(**data) << (--bytes_in_limb * 8);
			(*data)++;
		}
	}
	return (n);
}

t_bn		*retrieve_nb_from_data(unsigned char **data,
									unsigned char *start,
									uint32_t length)
{
	t_bn		*n;
	uint32_t	len;

	n = NULL;
	if (*data + 1 > start + length)
		return (NULL);
	(*data)++;
	len = get_number_len(data);
	if (**data == 0)
	{
		if (*data + 1 > start + length)
			return (NULL);
		(*data)++;
		len--;
	}
	if (*data + len <= start + length)
		n = get_bn(data, len);
	return (n);
}

static int	handle_private_data(t_rsa_data *rsa_data,
								unsigned char *ptr,
								unsigned char *data,
								uint32_t len)
{
	if (!(rsa_data->private_exp = retrieve_nb_from_data(&ptr, data, len)))
		return (1);
	if (!(rsa_data->prime1 = retrieve_nb_from_data(&ptr, data, len)))
		return (1);
	if (!(rsa_data->prime2 = retrieve_nb_from_data(&ptr, data, len)))
		return (1);
	if (!(rsa_data->exponent1 = retrieve_nb_from_data(&ptr, data, len)))
		return (1);
	if (!(rsa_data->exponent2 = retrieve_nb_from_data(&ptr, data, len)))
		return (1);
	if (!(rsa_data->coef = retrieve_nb_from_data(&ptr, data, len)))
		return (1);
	return (0);
}

int			parse_decoded_data(t_rsa_data *rsa_data,
								unsigned char *data,
								uint32_t len,
								int opts)
{
	unsigned char	*ptr;
	uint32_t		cur_len;

	ptr = data;
	if (*ptr++ != 0x30)
		return (1);
	cur_len = get_number_len(&ptr);
	if (cur_len + ptr > data + len)
		return (1);
	ptr += 3;
	if (!(rsa_data->modulus = retrieve_nb_from_data(&ptr, data, len)))
		return (1);
	if (!(rsa_data->public_exp = retrieve_nb_from_data(&ptr, data, len)))
		return (1);
	if ((opts & RSA_PUBIN) == 0)
	{
		if (handle_private_data(rsa_data, ptr, data, len) == 1)
		{
			ft_dprintf(STDERR_FILENO, "ft_ssl: Unable to load private key\n");
			return (1);
		}
	}
	return (0);
}
