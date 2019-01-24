/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:19:50 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/17 12:10:15 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

uint32_t	rotleft(uint32_t x, uint32_t n)
{
	return ((x << n) | (x >> (32 - n)));
}

uint32_t	rotright(uint32_t x, uint32_t n)
{
	return ((x >> n) | (x << (32 - n)));
}

uint64_t	shift_left(uint32_t key, uint8_t x)
{
	return ((key >> (28 - x)) | ((key << x) & 0xFFFFFFF));
}

static int	nb_digit(unsigned long long value, int size_base)
{
	int	digit;

	digit = 0;
	if (value == 0)
		return (1);
	while (value)
	{
		value /= size_base;
		digit++;
	}
	return (digit);
}

char		*itoua_base_llu(uint64_t value, char *base)
{
	char	*s;
	int		i;
	int		size_base;

	size_base = ft_strlen(base);
	i = nb_digit(value, size_base);
	if ((s = (char*)malloc((i + 1) * sizeof(char))) == NULL)
		return (NULL);
	s[i--] = '\0';
	if (value == 0)
		s[i] = '0';
	while (value)
	{
		s[i--] = base[value % size_base];
		value /= size_base;
	}
	return (s);
}
