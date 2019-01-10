/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_decode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 10:48:40 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/10 10:48:42 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ssl.h"

static void	des_message_decode_reg(t_des *des)
{
	int 			ret;
	int 			offset;

	while ((ret = read(des->fd[IN], des->input, BUF_SIZE)) > 0)
	{
		offset = 0;
		(ret % DES_BLOCK_SIZE) ? ft_error_msg("ft_ssl: bad block lenght") : 0;
		while (offset < ret)
		{
			des_get_cipher(des, offset);
			offset += DES_BLOCK_SIZE;
		}
		if (ret < BUF_SIZE)
			write(des->fd[OUT], des->input, ret - des->input[offset - 1]);
		else
			write(des->fd[OUT], des->input, offset);
	}
	(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
}

static void	des_message_decode_base64(t_des *des)
{
	int 			ret;
	int 			p_ret;
	int 			offset;
	int 			len;

	while ((ret = read(des->fd[IN], des->input, BASE64_BUF_SIZE)) > 0)
	{
		offset = 0;
		len = base64_decode(des->input, ret, des->fd[OUT], 1);
		while (offset < len)
		{
			des_get_cipher(des, offset);
			offset += DES_BLOCK_SIZE;
		}
		if (ret < BASE64_BUF_SIZE)
			write(des->fd[OUT], des->input, len - des->input[len - 1]);
		else
			write(des->fd[OUT], des->input, len);
		p_ret = ret;
	}
	(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
	if (p_ret == BASE64_BUF_SIZE && len < BUF_SIZE) 
		ft_error_msg("ft_ssl: Invalid character in input stream");
}

void 	des_message_decode(t_des *des)
{
	if (des->opts & DES_OPT_A)
		des_message_decode_base64(des);
	else
		des_message_decode_reg(des);
}
