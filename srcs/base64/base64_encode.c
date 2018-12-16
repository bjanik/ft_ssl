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

static void		encode_data(t_base64 *base, unsigned char b[])
{
	base->encoded[0] = (b[0] >> 2) & 0xFF;
	base->encoded[1] = ((b[0] & 0x03) << 4) | ((b[1] >> 4) & 0xFF);
	base->encoded[2] = ((b[1] & 0x0F) << 2) | ((b[2] >> 6) & 0xFF);
	base->encoded[3] = b[2] & 0x3F;
}

static void		put_encoded_data_to_buffer(t_base64 *base,
										char buffer[],
										int *buflen,
										int ret)
{
	int	i;

	i = -1;
	while (++i < 4)
	{
		if (ret < QUANTUM_SIZE && ret < i && base->encoded[i] == 0)
			buffer[*buflen + i] = '=';
		else
			buffer[*buflen + i] = B64[base->encoded[i]];
	}
	*buflen += 4;
}

void		encode_data2(t_base64 *base)
{
	int				ret;
	int 			buflen;
	char			buffer[BUF_SIZE + 1];
	unsigned char	b[QUANTUM_SIZE];

	buflen = 0;
	ft_memset(buffer, 0x0, BUF_SIZE + 1);
	while ((ret = read(base->fd[IN], b, QUANTUM_SIZE)) > 0)       
	{
		encode_data(base, b);
		put_encoded_data_to_buffer(base, buffer, &buflen, ret);
		if (buflen == BUF_SIZE)
		{
			write(base->fd[OUT], buffer, buflen);
			ft_memset(buffer, 0x0, BUF_SIZE + 1);
			buflen = 0;
		}
		ft_memset(b, 0x0, QUANTUM_SIZE);
	}
	(ret < 0) ?	ft_error_msg("ft_ssl: Read error") : 0;
	write(base->fd[OUT], buffer, buflen);
	ft_putchar('\n');
}