/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bn_cmp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:26:01 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 13:26:02 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"

int		bn_cmp(t_bn *a, t_bn *b)
{
	int64_t i;

	if (a->size > b->size)
		return (1);
	if (a->size < b->size)
		return (-1);
	i = a->size;
	while (--i >= 0)
	{
		if (a->num[i] > b->num[i])
			return (1);
		if (a->num[i] < b->num[i])
			return (-1);
	}
	return (0);
}

int		bn_cmp_ui(t_bn *n, uint64_t ui)
{
	if (n->size > 1)
		return (1);
	if (n->size == 0 && ui == 0)
		return (0);
	if (n->size == 1 && n->num[0] == ui)
		return (0);
	if (n->size == 1 && n->num[0] > ui)
		return (1);
	if (n->size == 1 && n->num[0] < ui)
		return (-1);
	return (1);
}
