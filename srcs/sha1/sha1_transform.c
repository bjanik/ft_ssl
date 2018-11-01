/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha1_transform.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 12:41:07 by bjanik            #+#    #+#             */
/*   Updated: 2018/10/31 12:41:08 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static void	get_word_sequence(t_ctx *ctx, uint32_t w[])
{
	int	i;

	i = -1;
	while (++i < 16)
	{
		w[i] = (ctx->block[i * 4] << 24) |
			(ctx->block[i * 4 + 1] << 16) |
			(ctx->block[i * 4 + 2] << 8) |
			ctx->block[i * 4 + 3];
	}
	while (i < 80)
	{
		w[i] = rotleft(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
		i++;
	}
}

static void	get_values(uint32_t h[], uint32_t *f, uint32_t *k, int i)
{
	if (i < 20)
	{
		*f = (h[1] & h[2]) | ((~h[1]) & h[3]);
		*k = 0x5a827999;
	}
	else if (i < 40)
	{
		*f = h[1] ^ h[2] ^ h[3];
		*k = 0x6ed9eba1;
	}
	else if (i < 60)
	{
		*f = (h[1] & h[2]) | (h[1] & h[3]) | (h[2] & h[3]);
		*k = 0x8f1bbcdc;
	}
	else
	{
		*f = h[1] ^ h[2] ^ h[3];
		*k = 0xca62c1d6;
	}
}

void		sha1_transform(t_ctx *ctx)
{
	uint32_t	h[6];
	uint32_t	w[80];
	uint32_t	k;
	uint32_t	f;
	int			i;

	i = -1;
	while (++i < 5)
		h[i] = ctx->h[i];
	get_word_sequence(ctx, w);
	i = -1;
	while (++i < 80)
	{
		get_values(h, &f, &k, i);
		h[5] = rotleft(h[0], 5) + f + h[4] + w[i] + k;
		h[4] = h[3];
		h[3] = h[2];
		h[2] = rotleft(h[1], 30);
		h[1] = h[0];
		h[0] = h[5];
	}
	i = -1;
	while (++i < 5)
		ctx->h[i] += h[i];
}
