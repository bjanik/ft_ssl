/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_bn.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:24:04 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 13:24:06 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"
#include "ft_ssl.h"

void	display_bn(t_bn *n)
{
	int64_t i;

	i = n->size;
	if (n->size == 0)
		printf("0");
	else
	{
		while (--i > -1)
		{
			if (i == n->size - 1)
				ft_printf("%llX", n->num[i]);
			else
				ft_printf("%016llX", n->num[i]);
		}
	}
	printf("\n");
}

void	power_of_two(t_bn *n, unsigned int pow)
{
	int64_t	limb;

	limb = pow / 64;
	while (limb >= n->alloc)
		bn_realloc(n);
	bn_set_zero(n);
	n->num[limb] = 1;
	n->num[limb] <<= pow % 64;
	n->size = limb + 1;
}

void	bn_set_bit(t_bn *n, uint64_t pos)
{
	int64_t	limb;

	limb = pos / 64;
	n->num[limb] |= (uint64_t)1 << (pos % 64);
	if (n->size < limb + 1)
		n->size = limb + 1;
}
