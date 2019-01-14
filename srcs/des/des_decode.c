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
	// int 			offset;
	int 			len;
	unsigned char 	buf[BASE64_BUF_SIZE + 1];

	len = 0;
	while ((ret = read(des->fd[IN], des->in, BUF_SIZE)) > 0)
	{
		// (ret % DES_BLOCK_SIZE) ? ft_error_msg("ft_ssl: bad block lenght") : 0;
		!(len + ret <= BUF_SIZE) ? ft_memcpy(buf + len, des->in, ret) : 
						ft_memcpy(buf + len, des->in, BUF_SIZE - len);
		len += ret;
		if (len >= BUF_SIZE)
		{
			des_get_cipher(des, BUF_SIZE, buf);
			if (len > BUF_SIZE)
			{
				ft_memcpy(buf, des->in + ret - len + BUF_SIZE, len - BUF_SIZE);
				len -= BUF_SIZE;
			}
			else
				len = 0;
		}
		// des_get_cipher(des, offset, buf);

		// while (offset < ret)
		// {
			
		// 	offset += DES_BLOCK_SIZE;
		// }
		// if (ret < BUF_SIZE)
		// 	write(des->fd[OUT], des->in, ret - des->in[offset - 1]);
		// else
		// 	write(des->fd[OUT], des->in, offset);
	}
	// des_final(des, 0, len, buf);
	(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
}

static void	des_message_decode_base64(t_des *des)
{
	int 			ret;
	int 			p_ret;
	int 			offset;
	int 			len;
	unsigned char 	buf[BASE64_BUF_SIZE + 1];

	while ((ret = read(des->fd[IN], des->in, BASE64_BUF_SIZE)) > 0)
	{
		offset = 0;
		len = base64_decode(des->in, ret, des->fd[OUT], 1);
		while (offset < len)
		{
			des_get_cipher(des, offset, buf);
			offset += DES_BLOCK_SIZE;
		}
		if (ret < BASE64_BUF_SIZE)
			write(des->fd[OUT], des->in, len - des->in[len - 1]);
		else
			write(des->fd[OUT], des->in, len);
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
