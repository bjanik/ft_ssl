/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 14:17:27 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 12:17:01 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static int			translate(unsigned char in[], size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (in[i] >= 65 && in[i] <= 90)
			in[i] -= 65;
		else if (in[i] >= 97 && in[i] <= 122)
			in[i] -= 71;
		else if (ft_isdigit(in[i]))
			in[i] += 4;
		else if (in[i] == '+')
			in[i] = 62;
		else if (in[i] == '/')
			in[i] = 63;
		else if (in[i] != '=' || (in[i] == '=' && i < 2))
		{
			ft_dprintf(STDERR_FILENO, "Invalid character in input stream\n");
			return (-1);
		}
		else if (in[i] == '=')
			return (i - 1);
		i++;
	}
	return (3);
}

int					decode(unsigned char in[], unsigned char out[], size_t len)
{
	int	ret;

	ret = translate(in, len);
	if (ret == -1)
		return (-1);
	out[0] = (in[0] << 2) | ((in[1] & 0x30) >> 4);
	out[1] = ((in[1] & 0x0F) << 4) | ((in[2] & 0x3C) >> 2);
	out[2] = ((in[2] & 0x03) << 6) | in[3];
	return (ret);
}

int					base64_decode(unsigned char in[], int ret, int fd,
									uint8_t des)
{
	int				offset;
	int				out_len;
	unsigned char	out[BUF_SIZE + 1];
	int				len;

	(ret % 4 != 0) ? ft_error_msg("ft_ssl: bad block length") : 0;
	offset = 0;
	out_len = 0;
	ft_memset(out, 0x0, BUF_SIZE + 1);
	len = 0;
	while (offset < ret)
	{
		len += decode(in + offset, out + out_len, 4);
		out_len += 3;
		offset += 4;
		if (len == BUF_SIZE && !des)
		{
			write(fd, out, BUF_SIZE);
			ft_memset(out, 0x0, BUF_SIZE);
			out_len = 0;
		}
	}
	(des == 1) ? (void)ft_memcpy(in, out, len) :
				(void)write(fd, out, len);
	return (len);
}

unsigned char		*base64_decode_data(uint32_t *decoded_data_len,
									char *data, uint32_t data_len)
{
	uint32_t		offset;
	unsigned char	*decoded_data;
	int				ret;

	if (data == NULL)
		return (NULL);
	if (!(decoded_data = (unsigned char *)malloc(data_len * sizeof(char))))
		return (NULL);
	offset = 0;
	while (offset < data_len)
	{
		ret = decode((unsigned char*)data + offset,
						decoded_data + *decoded_data_len, 4);
		if (ret == -1)
		{
			ft_strdel((char**)&decoded_data);
			break ;
		}
		*decoded_data_len += ret;
		offset += 4;
	}
	ft_strdel(&data);
	return (decoded_data);
}
