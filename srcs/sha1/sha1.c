/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 12:25:51 by bjanik            #+#    #+#             */
/*   Updated: 2018/11/01 15:25:54 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void		sha1_init(t_ctx *ctx)
{
	ctx->h[0] = 0x67452301;
	ctx->h[1] = 0xefcdab89;
	ctx->h[2] = 0x98badcfe;
	ctx->h[3] = 0x10325476;
	ctx->h[4] = 0xc3d2e1f0;
	ctx->len = 0;
	ctx->bitlen = 0;
	ctx->digest_len = SHA1_DIGEST_LEN;
	ft_memset(ctx->block, 0x0, BLOCK_SIZE);
	ft_memset(ctx->digest, 0x0, SHA1_DIGEST_LEN);
	ft_memset(ctx->cmd_name, 0x0, MAX_CMD_NAME_LEN);
	ft_memcpy(ctx->cmd_name, "SHA1", 4);
	ctx->transform = sha1_transform;
}

static void	sha1_final(t_ctx *ctx)
{
	int		i;

	i = -1;
	pad_message(ctx);
	while (++i < 8)
		ctx->block[63 - i] = ctx->bitlen >> i * 8;
	ctx->transform(ctx);
	i = -1;
	while (++i < 4)
	{
		ctx->digest[i] = (ctx->h[0] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 4] = (ctx->h[1] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 8] = (ctx->h[2] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 12] = (ctx->h[3] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 16] = (ctx->h[4] >> (24 - i * 8)) & 0x000000ff;
	}
}

void		sha1(t_msg *msg, uint32_t opts)
{
	t_ctx	ctx;

	sha1_init(&ctx);
	if (update(&ctx, msg, opts) == 0)
	{
		sha1_final(&ctx);
		output_digest(msg, ctx, opts);
	}
}
