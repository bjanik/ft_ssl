/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bn_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:27:45 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 13:27:46 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"
#include "libft.h"

int	bn_realloc(t_bn *n)
{
	uint64_t	*tmp;
	int64_t		i;

	tmp = n->num;
	n->num = (uint64_t*)ft_malloc(sizeof(uint64_t) * n->alloc * 2);
	if (n->num == NULL)
		return (1);
	ft_memset(n->num, 0, n->alloc * 2 * sizeof(uint64_t));
	i = -1;
	while (++i < n->size)
		n->num[i] = tmp[i];
	n->alloc *= 2;
	free(tmp);
	return (0);
}

int	bn_realloc_size(t_bn *n, int64_t new_alloc)
{
	uint64_t	*tmp;

	tmp = n->num;
	n->num = (uint64_t*)ft_malloc(sizeof(uint64_t) * new_alloc);
	if (n->num == NULL)
		return (1);
	ft_memset(n->num, 0, new_alloc * sizeof(uint64_t));
	ft_memcpy((void*)n->num, (void*)tmp, n->size * 8);
	n->alloc = new_alloc;
	free(tmp);
	return (0);
}

int	bn_realloc_size_zero(t_bn *n, int64_t new_alloc)
{
	int64_t	i;

	free(n->num);
	n->num = (uint64_t*)ft_malloc(sizeof(uint64_t) * new_alloc);
	if (n->num == NULL)
		return (1);
	i = -1;
	while (++i < new_alloc)
		n->num[i] = 0;
	n->alloc = new_alloc;
	return (0);
}
