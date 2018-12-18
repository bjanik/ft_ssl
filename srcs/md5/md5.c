/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:19:09 by bjanik            #+#    #+#             */
/*   Updated: 2018/11/01 15:39:51 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void					md5_init(t_ctx *ctx)
{
	ctx->len = 0;
	ctx->bitlen = 0;
	ctx->h[0] = 0x67452301;
	ctx->h[1] = 0xefcdab89;
	ctx->h[2] = 0x98badcfe;
	ctx->h[3] = 0x10325476;
	ctx->digest_len = MD5_DIGEST_LEN;
	ft_memset(ctx->block, 0x0, BLOCK_SIZE);
	ft_memset(ctx->digest, 0x0, MD5_DIGEST_LEN);
	ft_memset(ctx->cmd_name, 0x0, MAX_CMD_NAME_LEN);
	ft_memcpy(ctx->cmd_name, "MD5", 3);
	ctx->transform = md5_transform;
}

static void				md5_final(t_ctx *ctx)
{
	int	i;

	i = -1;
	pad_message(ctx);
	while (++i < 8)
		ctx->block[56 + i] = ctx->bitlen >> i * 8;
	ctx->transform(ctx);
	i = -1;
	while (++i < 4)
	{
		ctx->digest[i] = (ctx->h[0] >> (i * 8)) & 0x000000ff;
		ctx->digest[i + 4] = (ctx->h[1] >> (i * 8)) & 0x000000ff;
		ctx->digest[i + 8] = (ctx->h[2] >> (i * 8)) & 0x000000ff;
		ctx->digest[i + 12] = (ctx->h[3] >> (i * 8)) & 0x000000ff;
	}
}

unsigned char	*md5_core(t_ctx	*ctx, t_msg *msg, uint32_t opts)
{
	if (update(ctx, msg, opts) == 0)
	{
		md5_final(ctx);
		write(1, ctx->digest, 32);
		return ((unsigned char*)ft_strdup((char*)ctx->digest));
	}
	return (NULL);
}

void					md5(t_msg *msg, uint32_t opts)
{
	t_ctx			ctx;
	unsigned char	*digest;

	md5_init(&ctx);
	if ((digest = md5_core(&ctx, msg, opts)))
	{	
		output_digest(msg, ctx, opts);
		ft_strdel((char**)&digest);
	}
}
