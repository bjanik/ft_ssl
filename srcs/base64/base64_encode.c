/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64_encode.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 11:43:04 by bjanik            #+#    #+#             */
/*   Updated: 2018/11/23 11:43:06 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	encode(unsigned char in[], char out[], int diff)
{
	uint8_t	data[4];
	uint8_t i;

	if (diff < 3)
		ft_memset(in + diff, 0, 3 - diff);
	data[0] = (in[0] >> 2) & 0xFF;
	data[1] = ((in[0] & 0x03) << 4) | ((in[1] >> 4) & 0xFF);
	data[2] = ((in[1] & 0x0F) << 2) | ((in[2] >> 6) & 0xFF);
	data[3] = in[2] & 0x3F;
	i = 0;
	while (i < 4)
	{
		if (data[i] == 0 && diff < 3 && i > diff)
			out[i] = '=';
		else
			out[i] = B64[data[i]];
		i++;
	}
}

void	base64_encode(unsigned char in[], int ret, int fd)
{
	int				offset;
	int				out_len;
	char			out[BUF_SIZE + 1];

	offset = 0;
	out_len = 0;
	ft_memset(out, 0x0, BUF_SIZE + 1);
	if (ret > BUF_SIZE)
		ret = BUF_SIZE;
	while (offset < ret)
	{
		encode(in + offset, out + out_len, ret - offset);
		offset += 3;
		out_len += 4;
		if (out_len == BUF_SIZE)
		{
			write(fd, out, BUF_SIZE);
			ft_memset(out, 0x0, BUF_SIZE + 1);
			out_len = 0;
		}
	}
	write(fd, out, out_len);
}

char 		*base64_encode_data(unsigned char *data, uint32_t data_len)
{
	uint32_t 		offset, data_encoded_len;
	char 			*data_encoded;

	if ((data_encoded = (char*)ft_memalloc(data_len * 2)) == NULL)
		return (NULL);
	offset = 0;
	data_encoded_len = 0;
	while (offset < data_len)
	{
		encode(data + offset, data_encoded + data_encoded_len, data_len - offset);
		offset += 3;
		data_encoded_len += 4;
	}
	return (data_encoded);
}

