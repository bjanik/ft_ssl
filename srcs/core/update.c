/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 11:34:27 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 19:35:41 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

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

static int			read_from_stdin(t_msg *msg, t_ctx *ctx, uint32_t opts)
{
	int			buflen;
	int			ret;
	char		c;

	buflen = 0;
	while ((ret = read(msg->fd, &c, 1)) > 0)
	{
		ctx->block[ctx->len++] = c;
		msg->buf[buflen++] = c;
		if (buflen == BUF_SIZE)
		{
			if (opts & OPT_P)
			{
				write(STDOUT_FILENO, msg->buf, BUF_SIZE);
				ft_memset(msg->buf, 0x0, BUF_SIZE + 1);
			}
			buflen = 0;
		}
		(ctx->len == BLOCK_SIZE) ? transform_block(ctx) : 0;
	}
	read_error(msg->input_file, ret);
	if (!msg->fd && (opts & OPT_P))
		write(STDOUT_FILENO, msg->buf, buflen);
	return (ret);
}

static int			read_from_file(t_msg *msg, t_ctx *ctx)
{
	int			ret;

	while ((ret = read(msg->fd, ctx->block + ctx->len, BLOCK_SIZE)) > 0)
	{
		ctx->len += ret;
		if (ctx->len == BLOCK_SIZE)
			transform_block(ctx);
	}
	close(msg->fd);
	read_error(msg->input_file, ret);
	return (ret);
}

int					update(t_ctx *ctx, t_msg *msg, uint32_t opts)
{
	uint32_t	i;

	i = 0;
	if (msg->str)
		while (i < msg->msg_len)
		{
			ctx->block[ctx->len++] = msg->str[i++];
			if (ctx->len == BLOCK_SIZE)
			{
				ctx->transform(ctx);
				ctx->len = 0;
				ctx->bitlen += BLOCK_SIZE * 8;
			}
		}
	else if (msg->input_file)
		return (read_from_file(msg, ctx));
	else
		return (read_from_stdin(msg, ctx, opts));
	return (0);
}
