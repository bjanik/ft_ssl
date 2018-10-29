/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:19:09 by bjanik            #+#    #+#             */
/*   Updated: 2018/10/29 19:48:24 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void				md5_init(t_md5ctx *md5ctx)
{
	md5ctx->len = 0;
	md5ctx->bitlen = 0;
	md5ctx->h[0] = 0x67452301;
	md5ctx->h[1] = 0xefcdab89;
	md5ctx->h[2] = 0x98badcfe;
	md5ctx->h[3] = 0x10325476;
	memset(md5ctx->block, 0x0, 64);
	memset(md5ctx->digest, 0x0, 16);
}

static void			md5_update(t_md5ctx *md5ctx, t_msg *msg)
{
	uint32_t	i;
	int			ret;

	i = 0;
	if (msg->msg)
		while (i < msg->msg_len)
		{
			md5ctx->block[md5ctx->len++] = msg->msg[i++];
			if (md5ctx->len == 64)
			{
				md5_transform(md5ctx);
				md5ctx->len = 0;
				md5ctx->bitlen += 512;
			}
		}
	else
	{
		while ((ret = read(msg->fd, md5ctx->block, 64)) > 0)
		{
			if (!msg->fd && !msg->input_file)
				printf("%s", md5ctx->block);
			if (ret == 64)
			{
				md5_transform(md5ctx);
				md5ctx->bitlen += 512;
			}
			md5ctx->len = ret;
		}
		close(msg->fd);
	}
}

static void			md5_final(t_md5ctx *md5ctx)
{
	int	i;

	md5ctx->block[md5ctx->len] = 0x80;
	if (md5ctx->len < 56)
		memset(md5ctx->block + md5ctx->len + 1, 0x0, 56 - md5ctx->len);
	else
	{
		memset(md5ctx->block + md5ctx->len + 1, 0x0, 64 - md5ctx->len);
		md5_transform(md5ctx);
		memset(md5ctx->block, 0x0, 56);
	}
	i = -1;
	md5ctx->bitlen += md5ctx->len * 8;
	while (++i < 8)
		md5ctx->block[56 + i] = md5ctx->bitlen >> i * 8;
	md5_transform(md5ctx);
	i = -1;
	while (++i < 4)
	{
		md5ctx->digest[i] = (md5ctx->h[0] >> (i * 8)) & 0x000000ff;
		md5ctx->digest[i + 4] = (md5ctx->h[1] >> (i * 8)) & 0x000000ff;
		md5ctx->digest[i + 8] = (md5ctx->h[2] >> (i * 8)) & 0x000000ff;
		md5ctx->digest[i + 12] = (md5ctx->h[3] >> (i * 8)) & 0x000000ff;
	}
}

void				output_md5(t_msg *msg, unsigned char digest[],
								uint32_t opts)
{
	if (!(opts & OPT_Q) && !(opts & OPT_R))
		msg->input_file ? printf("MD5 (%s) = ", msg->input_file) :
			printf("MD5 (\"%s\") = ", msg->msg);
	print_hash(digest, 16);
	if (opts & OPT_R && !(opts & OPT_Q))
		printf("\"%s\"\n", (msg->input_file) ? msg->input_file : msg->msg);
}

void				md5(t_msg *msg, uint32_t opts)
{
	t_md5ctx	md5ctx;

	md5_init(&md5ctx);
	md5_update(&md5ctx, msg);
	md5_final(&md5ctx);
	output_md5(msg, md5ctx.digest, opts);
}
