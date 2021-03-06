/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bn_set.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:27:54 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 13:27:56 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "bn.h"

int		bn_set_random(t_bn *n, int64_t size)
{
	int			to_read;
	int			fd;
	uint32_t	bits;

	to_read = 0;
	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
		return (1);
	if (size % 8)
		to_read++;
	to_read += size / 8;
	if (read(fd, (void*)n->num, to_read) < 0)
		return (1);
	n->size = size / 64;
	if (size % 64 && n->num[n->size])
		n->size++;
	bits = bn_get_bit_number(n);
	if (bits > size)
		bn_shift_right(n, bits - size);
	else if (bits < size)
		bn_shift_left(n, size - bits);
	close(fd);
	return (0);
}

void	bn_set_zero(t_bn *n)
{
	int64_t	i;

	i = -1;
	if (n)
	{
		while (++i < n->alloc)
			n->num[i] = 0;
		n->size = 0;
	}
}

void	bn_set_ui(t_bn *n, uint64_t ui)
{
	bn_set_zero(n);
	if (ui > 0)
	{
		n->size = 1;
		n->num[0] = ui;
	}
}
