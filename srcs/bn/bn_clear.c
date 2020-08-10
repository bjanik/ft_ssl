/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bn_clear.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:25:32 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 13:25:42 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdlib.h>
#include "bn.h"

void	bn_clear(t_bn **n)
{
	if (n && *n)
	{
		free((*n)->num);
		free(*n);
		*n = NULL;
	}
}

void	bn_clears(int num, ...)
{
	va_list	args;
	t_bn	**n;
	int		i;

	va_start(args, num);
	i = -1;
	while (++i < num)
	{
		n = va_arg(args, t_bn **);
		bn_clear(n);
	}
	va_end(args);
}
