/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 12:08:53 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/11 12:08:56 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static void			decode_data(t_base64 *base)
{
	int				ret;
	unsigned char 	c;
	size_t 			len;
	int 			i;

	len = 0;
	i = 0;
	while ((ret = read(base->fd[IN], &c, 1)) > 0)
	{
		if (c == '\n' || c == '=')
			continue ;
		base->encoded[len++] = c;
		if (len == 4)
		{
			i++;
			decode(base->encoded, base->decoded, len);
			write(base->fd[OUT], base->decoded, 3);
			len = 0;
		}
	}
	if (len && i)
	{
		decode(base->encoded, base->decoded, len);
	 	write(base->fd[OUT], base->decoded, len - 1);
	}
}

int		base64_core(char **argv, t_base64 *base)
{
	int	ret;

	base64_opts(argv, base);
	if (base->opts & B_OPT_D)
		decode_data(base);
	else
	{

		while ((ret = read(base->fd[IN], base->buffer, BUF_SIZE)) > 0)
			base64_encode(base->buffer, ret, base->fd[OUT]);
		(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
		ft_putchar_fd('\n', base->fd[OUT]);
	}
	return (0);
}