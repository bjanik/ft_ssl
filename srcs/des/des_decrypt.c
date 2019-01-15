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

static void	des_decrypt_message_reg(t_des *des)
{
	int 			ret;
	int 			len;
	unsigned char 	buf[BASE64_BUF_SIZE + 1];

	len = 0;
	while ((ret = read(des->fd[IN], des->in, BUF_SIZE)) > 0)
	{
		
		!(len + ret <= BUF_SIZE) ? ft_memcpy(buf + len, des->in, ret) : 
						ft_memcpy(buf + len, des->in, BUF_SIZE - len);
		len += ret;
		if (len >= BUF_SIZE)
		{
			des_get_cipher(des, BUF_SIZE, buf);
			write(des->fd[OUT], buf, BUF_SIZE);
			if (len > BUF_SIZE)
				ft_memcpy(buf, des->in + ret - len + BUF_SIZE, len - BUF_SIZE);
			len = (len > BASE64_BUF_SIZE) ? len - BUF_SIZE : 0;
		}
	}
	(len % DES_BLOCK_SIZE) ? ft_error_msg("ft_ssl: bad block lenght") : 0;
	des_get_cipher(des, len, buf);
	write(des->fd[OUT], buf, len - buf[len - 1]);
	(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
}

static void	des_decrypt_message_base64(t_des *des)
{
	int 			ret;
	int 			len;
	int 			length;
	unsigned char 	buf[BASE64_BUF_SIZE + 1];

	len = 0;
	while ((ret = read(des->fd[IN], des->in, BASE64_BUF_SIZE)) > 0)
	{
		!(len + ret <= BASE64_BUF_SIZE) ? ft_memcpy(buf + len, des->in, ret) : 
						ft_memcpy(buf + len, des->in, BASE64_BUF_SIZE - len);
		len += ret;
		if (len >= BASE64_BUF_SIZE)
		{
			length = base64_decode(buf, BASE64_BUF_SIZE, des->fd[OUT], 1);
			des_get_cipher(des, length, buf);
			write(des->fd[OUT], buf, length);
			if (len > BUF_SIZE)
				ft_memcpy(buf, des->in + ret - len + BASE64_BUF_SIZE, len - BASE64_BUF_SIZE);
			len = (len > BASE64_BUF_SIZE) ? len - BASE64_BUF_SIZE : 0;
		}
	}
	(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
	length = base64_decode(buf, len, des->fd[OUT], 1);
	des_get_cipher(des, length, buf);
	write(des->fd[OUT], buf, length - buf[length - 1]);
	// if (p_ret == BASE64_BUF_SIZE && len < BUF_SIZE) 
	// 	ft_error_msg("ft_ssl: Invalid character in input stream");
}

void 	des_decrypt_message(t_des *des)
{
	if (des->opts & DES_OPT_A)
		des_decrypt_message_base64(des);
	else
		des_decrypt_message_reg(des);
}
