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

static void			sha256_init(t_sha256ctx *ctx)
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
	memset(ctx->block, 0x0, 64);
	memset(ctx->digest, 0x0, 32);
}

static	void		sha256_update(t_sha256ctx *ctx, t_msg *msg)
{
	uint32_t	i;
	int			ret;

	i = 0;
	if (msg->msg)
		while (i < msg->msg_len)
		{
			ctx->block[ctx->len++] = msg->msg[i++];
			if (ctx->len == 64)
			{
				sha256_transform(ctx);
				ctx->len = 0;
				ctx->bitlen += 512;
			}
		}
	else
		while ((ret = read(msg->fd, ctx->block, 64)) > 0)
		{
			if (!msg->fd && !msg->input_file)
				ft_printf("%s", ctx->block);
			if (ret == 64)
			{
				sha256_transform(ctx);
				ctx->bitlen += 512;
			}
			ctx->len = ret;
		}
}

static void			sha256_final(t_sha256ctx *ctx)
{
	int		i;

	ctx->block[ctx->len] = 0x80;
	if (ctx->len < 56)
		ft_memset(ctx->block + ctx->len + 1, 0x0, 56 - ctx->len);
	else
	{
		ft_memset(ctx->block + ctx->len + 1, 0x0, 64 - ctx->len);
		sha256_transform(ctx);
		ft_memset(ctx->block, 0x0, 56);
	}
	i = -1;
	ctx->bitlen += ctx->len * 8;
	while (++i < 8)
		ctx->block[63 - i] = ctx->bitlen >> i * 8;
	sha256_transform(ctx);
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

static void			output_sha256(t_msg *msg, unsigned char digest[],
									uint32_t opts)
{if (!(opts & OPT_Q) && !(opts & OPT_R) && (msg->input_file || msg->msg))
		msg->input_file ? ft_printf("SHA256 (%s) = ", msg->input_file) :
			ft_printf("SHA256 (\"%s\") = ", msg->msg);
	print_hash(digest, 32);
	if (opts & OPT_R && !(opts & OPT_Q) && (msg->input_file || msg->msg))
		msg->input_file ? ft_printf(" %s", msg->input_file) : 
						ft_printf(" \"%s\"", msg->msg);
	ft_putchar('\n');
}

void				sha256(t_msg *msg, uint32_t opts)
{
	t_sha256ctx		sha256ctx;

	sha256_init(&sha256ctx);
	sha256_update(&sha256ctx, msg);
	sha256_final(&sha256ctx);
	output_sha256(msg, sha256ctx.digest, opts);
}
