/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bn_add.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:26:52 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 13:26:57 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"

static void	add_loops(t_bn *res, t_bn *max, t_bn *min)
{
	uint64_t	tmp;
	int64_t		carry;
	int64_t		i;

	i = -1;
	carry = 0;
	while (++i < min->size)
	{
		tmp = max->num[i] + min->num[i] + carry;
		if (carry || min->num[i])
			carry = (tmp <= max->num[i]) ? 1 : 0;
		res->num[i] = tmp;
		(i >= res->size) ? res->size++ : 0;
	}
	while (i < max->size)
	{
		tmp = max->num[i] + carry;
		carry = (tmp < max->num[i]) ? 1 : 0;
		res->num[i] = tmp;
		(i >= res->size) ? res->size++ : 0;
		i++;
	}
	res->num[i] = carry;
	(i == res->size && carry) ? res->size++ : 0;
}

int			bn_add(t_bn *res, t_bn *op1, t_bn *op2)
{
	t_bn	*max;
	t_bn	*min;

	max = op1;
	min = op2;
	if (op1->size < op2->size)
	{
		max = op2;
		min = op1;
	}
	add_loops(res, max, min);
	return (0);
}

int			bn_add_ui(t_bn *n, uint64_t ui)
{
	int64_t i;

	if (n->size == 0)
	{
		n->num[n->size++] = ui;
		return (0);
	}
	i = -1;
	while (++i < n->size)
	{
		if (n->num[i] + ui < n->num[i])
		{
			n->num[i] += ui;
			ui = 1;
			(n->size > n->alloc) ? bn_realloc(n) : 0;
		}
		else
		{
			n->num[i] += ui;
			ui = 0;
			break ;
		}
	}
	(ui == 1) ? n->num[n->size] = ui : 0;
	return (0);
}
