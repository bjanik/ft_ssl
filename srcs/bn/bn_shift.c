/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bn_shift.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:28:02 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 13:28:03 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"
#include "libft.h"

void	bn_shift_right(t_bn *n, int64_t shift)
{
	uint64_t	lost[2];
	int64_t		i;

	ft_memset((void*)lost, 0, sizeof(uint64_t) * 2);
	if (shift / 64 >= n->size)
		bn_set_zero(n);
	else if (shift)
	{
		i = -1;
		while (++i < n->size && (shift / 64) > 0)
			n->num[i] = (i < shift / 64) ? n->num[i + shift / 64] : 0;
		n->size -= shift / 64;
		i = n->size;
		while (--i >= 0)
		{
			lost[0] = lost[1];
			lost[1] = n->num[i] << (64 - shift % 64) >> (64 - shift % 64);
			n->num[i] >>= shift % 64;
			n->num[i] |= lost[0] << (64 - (shift % 64));
			(i == n->size - 1 && n->num[i] == 0) ? n->size-- : 0;
		}
	}
}

void	bn_shift_left(t_bn *n, int64_t shift)
{
	uint64_t	lost[2];
	int64_t		i;

	if (n->size == 0)
		return ;
	ft_memset((void*)lost, 0, sizeof(uint64_t) * 2);
	if (shift)
	{
		i = n->size;
		while (--i >= 0 && shift / 64 > 0)
			n->num[i] = (i >= shift / 64) ? n->num[i - shift / 64] : 0;
		i = -1;
		while (++i < n->size)
		{
			lost[0] = lost[1];
			lost[1] = n->num[i] >> (64 - shift % 64);
			n->num[i] <<= shift % 64;
			n->num[i] |= lost[0];
		}
		if (n->size >= n->alloc)
			bn_realloc(n);
		n->num[i] |= lost[1];
		if (n->num[i])
			n->size++;
	}
}
