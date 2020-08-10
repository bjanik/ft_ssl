/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bn_mod.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:27:10 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 13:27:12 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"

void	bn_mod(t_bn *r, t_bn *n, t_bn *d)
{
	t_bn	*cn;
	t_bn	*cr;
	int		i;

	if (bn_cmp(n, d) < 0)
	{
		if (r != n)
			bn_copy(r, n);
		return ;
	}
	cn = bn_clone(n);
	cr = bn_clone(d);
	bn_set_zero(r);
	i = cn->size * 64;
	while (--i >= 0)
	{
		bn_shift_left(r, 1);
		r->num[0] ^= bn_get_bit(cn, i);
		if (r->size == 0 && r->num[0] > 0)
			INC_SIZE(r);
		(r->size > r->alloc) ? bn_realloc(r) : 0;
		bn_copy(cr, r);
		(bn_cmp(r, d) >= 0) ? bn_sub(r, cr, d) : 0;
	}
	bn_clears(2, &cn, &cr);
}
