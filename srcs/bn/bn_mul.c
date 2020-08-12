/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bn_mul.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:27:38 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 13:27:39 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"

static int	bn_concat(t_bn *n, t_bn *low, t_bn *high)
{
	if (n->alloc < low->size + high->size)
		bn_realloc_size(n, low->size + high->size + 1);
	bn_set_zero(n);
	while (n->size < low->size)
	{
		n->num[n->size] = low->num[n->size];
		n->size++;
	}
	while (n->size < low->size + high->size)
	{
		n->num[n->size] = high->num[n->size - low->size];
		n->size++;
	}
	return (0);
}

void		bn_mul(t_bn *res, t_bn *m, t_bn *q)
{
	t_bn		*a;
	t_bn		*cq;
	t_bn		*cm;
	t_bn		*ca;
	int64_t		i;

	a = (q->size >= m->size) ? bn_init_size(q->size * 64 + 1) :
								bn_init_size(m->size * 64 + 1);
	cq = bn_clone(q);
	cm = bn_clone(m);
	ca = bn_clone(a);
	i = -1;
	while (++i < q->size * 64)
	{
		bn_copy(ca, a);
		if (cq->num[0] & 1)
			bn_add(a, ca, cm);
		bn_shift_right(cq, 1);
		cq->size = q->size;
		if (a->num[0] & 1)
			cq->num[q->size - 1] |= 0x8000000000000000;
		bn_shift_right(a, 1);
	}
	bn_concat(res, cq, a);
	bn_clears(4, &a, &cq, &cm, &ca);
}
