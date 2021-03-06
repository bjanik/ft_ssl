/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bn_sub.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:28:10 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 13:28:18 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "bn.h"

static void	final_sub(t_bn *res)
{
	int64_t		i;

	i = res->alloc;
	while (--i >= 0)
	{
		if (res->num[i] != 0)
		{
			res->size = i + 1;
			break ;
		}
	}
}

void		bn_sub(t_bn *res, t_bn *a, t_bn *b)
{
	int64_t		j;
	int64_t		i;

	if (bn_cmp(a, b) < 0)
		return ;
	bn_set_zero(res);
	i = -1;
	while (++i < a->size)
	{
		if (i > b->size - 1)
			res->num[i] = a->num[i];
		else
			res->num[i] = a->num[i] - b->num[i];
		if (i < b->size && b->num[i] && res->num[i] >= a->num[i])
		{
			j = i + 1;
			while (a->num[j] == 0 && j < a->size - 2)
				a->num[j++] = ULLONG_MAX;
			a->num[j] -= 1;
		}
	}
	final_sub(res);
}

void		bn_sub_ui(t_bn *res, t_bn *a, uint64_t ui)
{
	t_bn		*ca;
	uint64_t	j;

	if (bn_cmp_ui(a, ui) < 0)
		return ;
	if (ui == 0)
	{
		bn_copy(res, a);
		return ;
	}
	ca = bn_clone(a);
	if (ca->num[0] - ui >= ca->num[0])
	{
		j = 1;
		while (ca->num[j] == 0)
			ca->num[j++] = ULLONG_MAX;
		ca->num[j] -= 1;
	}
	bn_copy(res, ca);
	res->num[0] = ca->num[0] - ui;
	bn_clear(&ca);
}
