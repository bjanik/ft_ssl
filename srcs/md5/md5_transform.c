/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_transform.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:19:26 by bjanik            #+#    #+#             */
/*   Updated: 2018/10/29 19:49:34 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static uint32_t	g_r[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

static uint32_t	g_k[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static void		get_sub_blocks(t_ctx *ctx, uint32_t m[])
{
	int	i;

	i = -1;
	while (++i < 16)
	{
		m[i] = ctx->block[i * 4] |
			(ctx->block[i * 4 + 1] << 8) |
			(ctx->block[i * 4 + 2] << 16) |
			(ctx->block[i * 4 + 3] << 24);
	}
}

static uint32_t	get_index(int i)
{
	if (i < 16)
		return (i);
	else if (i < 32)
		return ((5 * i + 1) % 16);
	else if (i < 48)
		return ((3 * i + 5) % 16);
	else
		return ((7 * i) % 16);
}

static void		operations_loop(uint32_t h[], uint32_t m[])
{
	uint32_t	f;
	int			i;

	i = -1;
	while (++i < 64)
	{
		if (i < 16)
			f = (h[1] & h[2]) | (~h[1] & h[3]);
		else if (i < 32)
			f = (h[3] & h[1]) | ((~h[3]) & h[2]);
		else if (i < 48)
			f = h[1] ^ h[2] ^ h[3];
		else
			f = h[2] ^ (h[1] | (~h[3]));
		h[4] = h[3];
		h[3] = h[2];
		h[2] = h[1];
		h[1] = rotleft(h[0] + f + g_k[i] + m[get_index(i)], g_r[i]) + h[1];
		h[0] = h[4];
	}
}

void			md5_transform(t_ctx *ctx)
{
	uint32_t	h[5];
	uint32_t	m[16];
	int			i;

	i = -1;
	while (++i < 4)
		h[i] = ctx->h[i];
	get_sub_blocks(ctx, m);
	operations_loop(h, m);
	i = -1;
	while (++i < 4)
		ctx->h[i] += h[i];
}
