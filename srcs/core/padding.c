/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 15:28:43 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 19:35:53 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	pad_message(t_ctx *ctx)
{
	ctx->block[ctx->len] = 0x80;
	if (ctx->len < 56)
		ft_memset(ctx->block + ctx->len + 1, 0x0, 56 - ctx->len);
	else
	{
		ft_memset(ctx->block + ctx->len + 1, 0x0, BLOCK_SIZE - ctx->len);
		ctx->transform(ctx);
		ft_memset(ctx->block, 0x0, 56);
	}
	ctx->bitlen += ctx->len * 8;
}
