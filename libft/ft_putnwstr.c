/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnwstr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/08 18:51:36 by bjanik            #+#    #+#             */
/*   Updated: 2017/01/09 16:31:18 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnwstr(wchar_t *str, int n, const int fd)
{
	int nb;

	nb = 0;
	while (*str)
	{
		if (nb >= n)
			break ;
		nb += ft_putwchar_t(*str++, fd);
	}
	return (nb);
}
