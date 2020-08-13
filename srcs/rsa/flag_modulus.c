/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_modulus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:24:28 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:24:31 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"
#include "ft_ssl.h"

static void	display_bn(t_bn *n, int fd)
{
	int64_t i;

	i = n->size;
	if (n->size == 0)
		ft_dprintf(fd, "0");
	else
	{
		while (--i > -1)
		{
			if (i == n->size - 1)
				ft_dprintf(fd, "%llX", n->num[i]);
			else
				ft_dprintf(fd, "%016llX", n->num[i]);
		}
	}
	ft_dprintf(fd, "\n");
}

void		flag_modulus(t_bn *n, int fd)
{
	ft_dprintf(fd, "Modulus=");
	display_bn(n, fd);
}
