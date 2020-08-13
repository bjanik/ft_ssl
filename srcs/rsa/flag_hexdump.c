/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_hexdump.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:22:19 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:22:22 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static void		print_ascii(const int fd,
							const unsigned char *msg,
							uint32_t len,
							int cur)
{
	uint32_t	i;

	i = 0;
	while (cur + i < len && i < 16)
	{
		if (ft_isprint(msg[cur + i]))
			ft_dprintf(fd, "%c", msg[cur + i]);
		else
			ft_dprintf(fd, ".");
		i++;
	}
}

static void		print_hex_values(const int fd,
									const unsigned char *msg,
									uint32_t len,
									int cur)
{
	uint32_t	i;

	i = 0;
	while (cur + i < len && i < 16)
	{
		if (i == 7 || ((i - 7) % 16) == 0)
			ft_dprintf(fd, "%02x-", msg[cur + i]);
		else
			ft_dprintf(fd, "%02x ", msg[cur + i]);
		i++;
	}
	ft_dprintf(fd, "%*c", (16 - i) * 3 + 2, ' ');
}

void			flag_hexdump(const int fd,
								const unsigned char *msg,
								uint32_t len)
{
	uint32_t	index;
	int			j;

	index = 0;
	j = 0;
	ft_dprintf(fd, "%04X - ", index);
	while (index < len)
	{
		if (index % 16 == 0 && index)
			ft_dprintf(fd, "\n%04X - ", index);
		print_hex_values(fd, msg, len, index);
		print_ascii(fd, msg, len, index);
		index += 16;
	}
	ft_putchar_fd('\n', fd);
}
