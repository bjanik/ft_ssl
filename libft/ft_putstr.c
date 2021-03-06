/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/03 16:59:50 by bjanik            #+#    #+#             */
/*   Updated: 2017/01/07 16:50:18 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr(char const *str)
{
	int	len;

	len = 0;
	if (str != NULL)
	{
		len = ft_strlen(str);
		write(1, str, len);
	}
	return (len);
}
