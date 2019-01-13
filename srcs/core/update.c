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

static void			read_error(char *input_file, int ret)
{
	if (ret == -1 && input_file)
	{
		write(STDERR_FILENO, "ft_ssl: ", 8);
		perror(input_file);
	}
	else if (ret == -1)
		perror("Read error");
}

static void			transform_block(t_ctx *ctx)
{
	ctx->transform(ctx);
	ctx->bitlen += BLOCK_SIZE * 8;
	ctx->len = 0;
}

static int			read_from_fd(t_msg *msg, t_ctx *ctx, uint32_t opts)
{
	int			buflen;
	int			ret;

	buflen = 0;
	while ((ret = read(msg->fd, ctx->block + ctx->len, BLOCK_SIZE)) > 0)
	{
		ctx->len += ret;
		if (!msg->fd && !msg->input_file)
		{
			ft_memcpy(msg->buf + buflen, ctx->block, ret);
			buflen += ret;
			if (buflen == BUF_SIZE)
				buflen = display_buf(msg, opts);
		}
		if (ctx->len == BLOCK_SIZE)
			transform_block(ctx);
	}
	(!msg->fd && !msg->input_file && opts & OPT_P) ? write(1, msg->buf, buflen)
													: 0;
	msg->input_file ? close(msg->fd) : 0;
	read_error(msg->input_file, ret);
	return (ret);
}

int					update(t_ctx *ctx, t_msg *msg, uint32_t opts)
{
	uint32_t		i;

	i = 0;
	if (msg->str)
		while (i < msg->msg_len)
		{
			ctx->block[ctx->len++] = msg->str[i++];
			if (ctx->len == BLOCK_SIZE)
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
