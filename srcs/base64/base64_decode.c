/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 14:17:27 by bjanik            #+#    #+#             */
/*   Updated: 2018/11/10 17:50:26 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#define ENCODE 0
#define DECODE 1

static void		translate(unsigned char in[])
{
	size_t 	i;

	i = 0;
	// printf("\nlen = %zu\n", len);
	while (i < 4)
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
		else if (in[i] != '=')
		{
			printf("invalid is: %c\n", in[i]);
			ft_error_msg("Invalid character in input stream");
		}
		i++;
	}
}

static void 	decode(unsigned char in[], unsigned char out[])
{
	// uint8_t		data[3];
	
	// (void)out;
	translate(in);
	out[0] = (in[0] << 2) | ((in[1] & 0x30) >> 4);
	// data[0] = (in[0] << 2) | ((in[1] & 0x30) >> 4);
	out[1] = ((in[1] & 0x0F) << 4) | ((in[2] & 0x3C) >> 2);
	// data[1] = ((in[1] & 0x0F) << 4) | ((in[2] & 0x3C) >> 2);
	out[2] = ((in[2] & 0x03) << 6) | in[3];
	// data[2] = ((in[2] & 0x03) << 6) | in[3];
	// write(1, out, 3);
}

int			base64_decode(unsigned char in[], int ret, int fd, uint8_t des)
{
	int				offset;
	int				out_len;
	unsigned char 	out[BUF_SIZE + 1];

	(ret % 4 != 0) ? ft_error_msg("ft_ssl: bad block length") : 0;
	offset = 0;
	out_len = 0;
	ft_memset(out, 0x0, BUF_SIZE + 1);
	while (offset < ret)
	{
		decode(in + offset, out + out_len);
		des ? ft_memcpy(in + offset , out + out_len, 3) : 0;
		out_len += 3;
		offset += 4;
		if (out_len == BUF_SIZE && !des)
		{
			write(fd, out, BUF_SIZE);
			ft_memset(out, 0x0, BUF_SIZE);
			out_len = 0;
		}
	}
	if (des)
		ft_memcpy(in, out, out_len);
	else
		write(fd, out, out_len);
	return (out_len);
}

// void			decode_data2(t_base64 *base)
// {
// 	int				ret;
// 	unsigned char 	b[4];
// 	unsigned char 	c;
// 	size_t 			len;
// 	int 			i;

// 	len = 0;
// 	i = 0;
// 	while ((ret = read(base->fd[IN], &c, 1)) > 0)
// 	{
// 		if (c == '\n' || c == '=')
// 			continue ;
// 		b[len++] = c;
// 		if (len == 4)
// 		{
// 			i++;
// 			decode_data(b, len);
// 			write(base->fd[OUT], base->decoded, 3);
// 			len = 0;
// 		}
// 	}
// 	if (len && i)
// 	{
// 		decode_data(b, len);
// 	 	write(base->fd[OUT], base->decoded, len - 1);
// 	}
// }