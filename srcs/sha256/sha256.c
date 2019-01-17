/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:19:35 by bjanik            #+#    #+#             */
/*   Updated: 2018/10/29 19:52:44 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void			sha256_init(t_ctx *ctx)
{
	ctx->h[0] = 0x6a09e667;
	ctx->h[1] = 0xbb67ae85;
	ctx->h[2] = 0x3c6ef372;
	ctx->h[3] = 0xa54ff53a;
	ctx->h[4] = 0x510e527f;
	ctx->h[5] = 0x9b05688c;
	ctx->h[6] = 0x1f83d9ab;
	ctx->h[7] = 0x5be0cd19;
	ctx->len = 0;
	ctx->bitlen = 0;
	ctx->digest_len = SHA256_DIGEST_LEN;
	ft_memset(ctx->block, 0x0, BLOCK_SIZE);
	ft_memset(ctx->digest, 0x0, SHA256_DIGEST_LEN);
	ft_memset(ctx->cmd_name, 0x0, MAX_CMD_NAME_LEN);
	ft_memcpy(ctx->cmd_name, "SHA256", 6);
	ctx->transform = sha256_transform;
}

void			sha256_final(t_ctx *ctx)
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
		ctx->digest[i + 20] = (ctx->h[5] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 24] = (ctx->h[6] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 28] = (ctx->h[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

unsigned char	*sha256_core(t_ctx *ctx, t_msg *msg, uint32_t opts)
{
	if (update(ctx, msg, opts) == 0)
	{
		sha256_final(ctx);
		return ((unsigned char*)ft_strdup((char*)ctx->digest));
	}
	return (NULL);
}

void				sha256(t_msg *msg, uint32_t opts)
{
	t_ctx			ctx;
	unsigned char	*digest;

	sha256_init(&ctx);
	if ((digest = sha256_core(&ctx, msg, opts)))
	{
		sha256_final(&ctx);
		output_digest(msg, ctx, opts);
	}
}
