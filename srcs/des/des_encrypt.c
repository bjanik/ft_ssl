/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_encrypt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 11:51:25 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 11:51:27 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

unsigned char		*des_encrypt_data(t_des *des,
										unsigned char *data,
										uint32_t *data_len)
{
	unsigned char	*data_encrypted;
	unsigned char	*data_with_pad;
	int				offset;
	int				data_encrypted_len;
	int				padding_len;

	offset = 0;
	padding_len = 8 - *data_len % 8;
	data_encrypted_len = *data_len + padding_len;
	if (!(data_with_pad = (unsigned char *)ft_malloc(data_encrypted_len)))
		return (NULL);
	ft_memcpy(data_with_pad, data, *data_len);
	ft_memset(data_with_pad + *data_len, padding_len, padding_len);
	data_encrypted = (unsigned char *)ft_malloc(data_encrypted_len);
	while (offset < data_encrypted_len)
	{
		cipher_to_string(des->des_mode[0](convert_input_to_block(data_with_pad
							+ offset), des), data_encrypted + offset);
		offset += DES_BLOCK_SIZE;
	}
	ft_memdel((void**)&data_with_pad);
	*data_len = data_encrypted_len;
	return (data_encrypted);
}
