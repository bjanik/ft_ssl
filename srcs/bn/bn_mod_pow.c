/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bn_mod_pow.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:27:20 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 13:27:21 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"

void	bn_mod_pow(t_bn *res, t_bn *b, t_bn *exp, t_bn *mod)
{
	t_bn	*cb;
	t_bn	*cexp;
	t_bn	*cmod;

	cb = bn_clone(b);
	cexp = bn_clone(exp);
	cmod = bn_clone(mod);
	bn_set_ui(res, 1);
	bn_mod(cb, cb, cmod);
	while (bn_cmp_ui(cexp, 0))
	{
		if (IS_ODD(cexp->num[0]))
		{
			bn_mul(res, res, cb);
			bn_mod(res, res, cmod);
		}
		bn_shift_right(cexp, 1);
		bn_mul(cb, cb, cb);
		bn_mod(cb, cb, cmod);
	}
	bn_clears(3, &cb, &cexp, &cmod);
}
