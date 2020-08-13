/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/30 15:07:58 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/13 19:55:19 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_malloc(size_t size)
{
	void	*ptr;

	if (!(ptr = malloc(size)))
	{
		ft_dprintf(STDERR_FILENO, "malloc failed\n");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}
