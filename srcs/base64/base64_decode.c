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

static void		translate(unsigned char b[], size_t len)
{
	size_t 	i;

	i = 0;
	while (i < len)
	{
		if (b[i] >= 65 && b[i] <= 90)
			b[i] -= 65;
		else if (b[i] >= 97 && b[i] <= 122)
			b[i] -= 71;
		else if (ft_isdigit(b[i]))
			b[i] += 4;
		else if (b[i] == '+')
			b[i] = 62;
		else if (b[i] == '/')
			b[i] = 63;		
		else if (b[i] != '=')
		{
			ft_putendl_fd("Invalid character", STDERR_FILENO);
			exit(1);
		}
		i++;
	}
}

static void 		decode_data(unsigned char decoded[], unsigned char b[], size_t len)
{
	translate(b, len);
	decoded[0] = (b[0] << 2) | ((b[1] & 0x30) >> 4);
	decoded[1] = ((b[1] & 0x0F) << 4) | ((b[2] & 0x3C) >> 2);
	decoded[2] = ((b[2] & 0x03) << 6) | b[3];
}

void			decode_data2(t_base64 *base)
{
	int				ret;
	unsigned char 	b[4];
	unsigned char 	c;
	size_t 			len;
	int 			i;

	len = 0;
	i = 0;
	while ((ret = read(base->fd[IN], &c, 1)) > 0)
	{
		if (c == '\n' || c == '=')
			continue ;
		b[len++] = c;
		if (len == 4)
		{
			i++;
			decode_data(base->decoded, b, len);
			write(base->fd[OUT], base->decoded, 3);
			len = 0;
		}
	}
	if (len && i)
	{
		decode_data(base->decoded, b, len);
	 	write(base->fd[OUT], base->decoded, len - 1);
	}
}

int			main(int argc, char **argv)
{
	t_base64 base;
	uint8_t	action;

	action = ENCODE;
	base.input_file = NULL;
	base.output_file = NULL;
	if (argv[1] && !ft_strcmp(argv[1], "-d"))
		action = DECODE;
	else if (argv[1])
		base.input_file = argv[1];
	if (argc > 2)
		base.input_file = argv[2];
	init_processing(&base);
	if (action == ENCODE)
		encode_data2(&base);
	else
		decode_data2(&base);
}