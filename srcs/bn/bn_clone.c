/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bn_clone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:25:48 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 13:25:56 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "bn.h"

t_bn	*bn_clone(t_bn *a)
{
	t_bn	*n;
	int64_t	i;

	n = (t_bn*)ft_malloc(sizeof(t_bn));
	if (n == NULL)
		return (NULL);
	n->num = (uint64_t*)ft_malloc(sizeof(uint64_t) * (a->alloc + 1));
	if (n->num == NULL)
	{
		free(n);
		return (NULL);
	}
	n->size = a->size;
	n->alloc = a->alloc;
	i = -1;
	while (++i < n->size)
		n->num[i] = a->num[i];
	return (n);
}
