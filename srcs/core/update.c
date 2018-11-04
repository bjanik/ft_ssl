/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 11:34:27 by bjanik            #+#    #+#             */
/*   Updated: 2018/11/01 14:15:12 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static int			display_buf(t_msg *msg, uint32_t opts)
{
	if (opts & OPT_P)
	{
		write(STDOUT_FILENO, msg->buf, BUF_SIZE);
		ft_memset(msg->buf, 0x0, BUF_SIZE + 1);
	}
	return (0);
}

static int			read_from_fd(t_msg *msg, t_ctx *ctx, uint32_t opts)
{
	int			buflen;
	int			ret;

	buflen = 0;
	while ((ret = read(msg->fd, ctx->block + ctx->len, 1)) > 0)
	{
		msg->buf[buflen++] = ctx->block[++ctx->len - 1];
		if (!msg->fd && !msg->input_file && buflen == BUF_SIZE)
			buflen = display_buf(msg, opts);
		if (ctx->len == 64)
		{
			ctx->transform(ctx);
			ctx->bitlen += 512;
			ctx->len = 0;
		}
	}
	(!msg->fd && !msg->input_file && opts & OPT_P) ? write(1, msg->buf, buflen)
													: 0;
	msg->input_file ? close(msg->fd) : 0;
	if (ret == -1 && msg->input_file)
	{
		write(STDERR_FILENO, "ft_ssl: ", 8);
		perror(msg->input_file);
	}
	return (ret);
}

int					update(t_ctx *ctx, t_msg *msg, uint32_t opts)
{
	uint32_t		i;

	i = 0;
	if (msg->msg)
		while (i < msg->msg_len)
		{
			ctx->block[ctx->len++] = msg->msg[i++];
			if (ctx->len == 64)
			{
				ctx->transform(ctx);
				ctx->len = 0;
				ctx->bitlen += 512;
			}
		}
	else
		return (read_from_fd(msg, ctx, opts));
	return (0);
}
