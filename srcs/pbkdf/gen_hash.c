/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_hash.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 18:10:18 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/12 18:10:19 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

unsigned char	*gen_hash(t_ctx *ctx, t_msg *msg, int des)
{
	unsigned char		*hash;

	if (des == SINGLE_DES)
	{
		md5_init(ctx);
		hash = md5_core(ctx, msg, 0);
	}
	else
	{
		sha256_init(ctx);
		hash = sha256_core(ctx, msg, 0);
	}
	return (hash);
}
