/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:19:50 by bjanik            #+#    #+#             */
/*   Updated: 2018/10/29 11:19:52 by bjanik           ###   ########.fr       */
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