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
	ft_memset(md5ctx->block, 0x0, 64);
	ft_memset(md5ctx->digest, 0x0, 16);
}

void				read_from_fd(t_msg *msg, t_md5ctx *md5ctx)
{
	unsigned char	buf[1024 + 1];
	int 			buf_len = 0;

	buf_len = 0;
	ft_memset(buf, 0x0, 1025);
	while (read(msg->fd, md5ctx->block + md5ctx->len, 1) > 0)
	{
		if (!msg->fd && !msg->input_file)
		{
			buf[buf_len++] = md5ctx->block[++md5ctx->len - 1];
			if (buf_len == 1024)
			{
				write(STDOUT_FILENO, buf, 1024);
				ft_memset(buf, 0x0, 1025);
				buf_len = 0;
			}
		}
		if (md5ctx->len == 64)
		{
			md5_transform(md5ctx);
			md5ctx->bitlen += 512;
			md5ctx->len = 0;
		}
	}
	(!msg->fd && !msg->input_file) ? write(STDOUT_FILENO, buf, buf_len) : 0;
	close(msg->fd);
}

static void			md5_update(t_md5ctx *md5ctx, t_msg *msg)
{
	uint32_t		i;

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
		read_from_fd(msg, md5ctx);
}

static void			md5_final(t_md5ctx *md5ctx)
{
	int	i;

	md5ctx->block[md5ctx->len] = 0x80;
	if (md5ctx->len < 56)
		ft_memset(md5ctx->block + md5ctx->len + 1, 0x0, 56 - md5ctx->len);
	else
	{
		ft_memset(md5ctx->block + md5ctx->len + 1, 0x0, 64 - md5ctx->len);
		md5_transform(md5ctx);
		ft_memset(md5ctx->block, 0x0, 56);
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
	if (!(opts & OPT_Q) && !(opts & OPT_R) && (msg->input_file || msg->msg))
		msg->input_file ? ft_printf("MD5 (%s) = ", msg->input_file) :
			ft_printf("MD5 (\"%s\") = ", msg->msg);
	print_hash(digest, 16);
	if (opts & OPT_R && !(opts & OPT_Q) && (msg->input_file || msg->msg))
		msg->input_file ? ft_printf(" %s", msg->input_file) : 
						ft_printf(" \"%s\"", msg->msg);
	ft_putchar('\n');
}

void				md5(t_msg *msg, uint32_t opts)
{
	t_md5ctx	md5ctx;
	union		u_ctx;

	u_ctx.md5ctx = &md5ctx;
	md5_init(&md5ctx);
	md5_update(u_ctx.md5ctx, msg);
	md5_final(&md5ctx);
	output_md5(msg, md5ctx.digest, opts);
}