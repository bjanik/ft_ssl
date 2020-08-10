/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bn_copy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:26:08 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 13:26:18 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"

int		bn_copy(t_bn *a, t_bn *b)
{
	int64_t i;

	if (a->alloc < b->size)
	{
		if (bn_realloc_size_zero(a, SIZE(b)))
			return (1);
	}
	i = -1;
	while (++i < b->size)
		a->num[i] = b->num[i];
	a->size = b->size;
	return (0);
}
