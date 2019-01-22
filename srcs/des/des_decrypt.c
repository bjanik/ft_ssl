/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_decode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 10:48:40 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/16 18:33:37 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

const unsigned char g_padding_patterns[][8] = {
	"\x01",
	"\x02\x02",
	"\x03\x03\x03",
	"\x04\x04\x04\x04",
	"\x05\x05\x05\x05\x05",
	"\x06\x06\x06\x06\x06\x06",
	"\x07\x07\x07\x07\x07\x07\x07",
	"\x08\x08\x08\x08\x08\x08\x08\x08",
};


int 	init_decryption_with_salt(t_des *des, unsigned char buf[], int *len)
{
	// unsigned char	keys[4][8];
	int 			ret;

	ret = 0;
	if (!(des->opts & DES_OPT_K))
	{
		ret = read(des->fd[IN], buf, 16);
		(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
		if (ret < 16)
		{
			ft_putendl_fd("ft_ssl: Bad decrypt", STDERR_FILENO);
			ret = 1;
		}	
		else if (!ft_strncmp((char*)buf, "Salted__", 8))
		{
			if (!(des->salt = (unsigned char*)malloc(sizeof(unsigned char) * 8)))
				ft_error_msg("ft_ssl: Malloc failed");
			ft_memcpy(des->salt, buf + 8, 8);
		}
		generate_keys_vector(des);
		ft_memdel((void**)&des->salt);
	}
	*len = 0;
	return (ret);
}

static int 	check_errors(unsigned char buf[], int len, int ret)
{
	(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
	if (buf[len - 1] < 9 &&	
		ft_memcmp(buf + len - buf[len - 1],
				  g_padding_patterns[buf[len - 1] - 1],
		 		  buf[len - 1]))
	{
		ft_putendl_fd("ft_ssl: Bad decrypt", STDERR_FILENO);
		return (1);
	}
	else if (len % DES_BLOCK_SIZE)
	{	
		ft_putendl_fd("ft_ssl: bad block lenght", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int des_decrypt_message_reg(t_des *des)
{
	int				ret;
	int				len;
	unsigned char	buf[BASE64_BUF_SIZE + 1];

	if (init_decryption_with_salt(des, buf, &len))
		return (1);
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
	if (check_errors(buf, len, ret))
		return (1);
	des_get_cipher(des, len, buf);
	write(des->fd[OUT], buf, len - buf[len - 1]);
	return (0);
}

static int	remove_newlines_from_input(unsigned char input[], int ret)
{
	uint16_t	i;
	uint16_t	j;

	i = 0;
	j = 0;
	while (i < ret)
	{
		if (input[i] != '\n' && input[i] != ' ')
			input[j++] = input[i];
		i++;
	}
	return (j);
}

static int	write_b64_decoding(t_des *des,
								unsigned char buf[],
								int len, int final)
{
	int	length;

	length = base64_decode(buf, len, des->fd[OUT], 1);
	if (length % DES_BLOCK_SIZE)
	{	
		ft_putendl_fd("ft_ssl: bad block lenght", STDERR_FILENO);
		return (1);
	}
	des_get_cipher(des, length, buf);
	if (final)
		write(des->fd[OUT], buf, length - buf[length - 1]);
	else
		write(des->fd[OUT], buf, length);
	return (0);
}

static int	des_decrypt_message_base64(t_des *des)
{
	int				ret;
	int				len;
	unsigned char	buf[BASE64_BUF_SIZE + 1];

	len = 0;
	while ((ret = read(des->fd[IN], des->in, BASE64_BUF_SIZE)) > 0)
	{
		ret = remove_newlines_from_input(des->in, ret);
		!(len + ret <= BASE64_BUF_SIZE) ? ft_memcpy(buf + len, des->in, ret) :
						ft_memcpy(buf + len, des->in, BASE64_BUF_SIZE - len);
		len += ret;
		if (len >= BASE64_BUF_SIZE)
		{
			write_b64_decoding(des, buf, BASE64_BUF_SIZE, 0);
			if (len > BUF_SIZE)
				ft_memcpy(buf, des->in + ret - len + BASE64_BUF_SIZE,
						len - BASE64_BUF_SIZE);
			len = (len > BASE64_BUF_SIZE) ? len - BASE64_BUF_SIZE : 0;
		}
	}
	(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
	if (write_b64_decoding(des, buf, len, 1))
		return (1);
	return (0);
}

int		des_decrypt_message(t_des *des)
{
	if (des->opts & DES_OPT_A)
		return (des_decrypt_message_base64(des));
	return (des_decrypt_message_reg(des));
}
