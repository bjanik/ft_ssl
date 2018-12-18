/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 12:27:37 by bjanik            #+#    #+#             */
/*   Updated: 2018/11/01 14:12:45 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	output_digest(t_msg *msg, t_ctx ctx, uint32_t opts)
{
	if (!(opts & OPT_Q) && !(opts & OPT_R) && (msg->input_file || msg->str))
	{
		if (msg->input_file)
			ft_printf("%s (%s) = ", ctx.cmd_name, msg->input_file);
		else
			ft_printf("%s (\"%s\") = ", ctx.cmd_name, msg->str);
	}
	print_hash(ctx.digest, ctx.digest_len, 0);
	if (opts & OPT_R && !(opts & OPT_Q) && (msg->input_file || msg->str))
	{
		if (msg->input_file)
			ft_printf(" %s", msg->input_file);
		else
			ft_printf(" \"%s\"", msg->str);
	}
	ft_putchar('\n');
}
