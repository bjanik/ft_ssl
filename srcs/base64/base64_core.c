/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 12:08:53 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 12:23:12 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static void	decode_data(t_base64 *base)
{
	int				ret;
	unsigned char	c;
	size_t			len;
	int				i;

	len = 0;
	i = 0;
	while ((ret = read(base->fd[IN], &c, 1)) > 0)
	{
		if (c == '\n' || c == '=' || c == ' ')
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

static int	output(t_base64 *base, unsigned char buf[], int len, int ret)
{
	len += ret;
	if (len >= BUF_SIZE)
	{
		base64_encode(base->buffer, len, base->fd[OUT]);
		if (len > BUF_SIZE)
			ft_memcpy(buf, base->buffer + ret - len + BUF_SIZE,
					  len - BUF_SIZE);
		len = (len > BUF_SIZE) ? len - BUF_SIZE : 0;
	}
	return (len);
}

int			base64_core(char **argv, t_base64 *base)
{
	int				ret;
	int				len;
	unsigned char	buf[BASE64_BUF_SIZE + 1];

	if (base64_opts(argv, base))
		return (1);
	if (base->opts & B_OPT_D)
		decode_data(base);
	else
	{
		len = 0;
		while ((ret = read(base->fd[IN], base->buffer, BUF_SIZE)) > 0)
		{
			!(len + ret <= BUF_SIZE) ? 
						ft_memcpy(buf + len, base->buffer, ret) :
						ft_memcpy(buf + len, base->buffer, BUF_SIZE - len);
			len = output(base, buf, len, ret);
		}
		base64_encode(buf, len, base->fd[OUT]);
		(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
		ft_putchar_fd('\n', base->fd[OUT]);
	}
	ft_memdel((void**)&base);
	return (0);
}
