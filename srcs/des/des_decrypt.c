/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_decode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 10:48:40 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 18:00:18 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

const unsigned char	g_padding_patterns[][8] = {
	"\x01",
	"\x02\x02",
	"\x03\x03\x03",
	"\x04\x04\x04\x04",
	"\x05\x05\x05\x05\x05",
	"\x06\x06\x06\x06\x06\x06",
	"\x07\x07\x07\x07\x07\x07\x07",
	"\x08\x08\x08\x08\x08\x08\x08\x08",
};

int				init_decryption_with_salt(t_des *des,
											unsigned char buf[],
											int *len)
{
	int	ret;

	ret = 0;
	if (!(des->opts & DES_OPT_K))
	{
		ret = read(des->fd[IN], buf, 16);
		(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
		if (ret < 16)
		{
			ft_putendl_fd("ft_ssl: error reading input file", STDERR_FILENO);
			return (1);
		}
		else if (!ft_strncmp((char*)buf, "Salted__", 8))
		{
			ft_memdel((void**)&des->salt);
			if (!(des->salt = (unsigned char*)ft_malloc(sizeof(unsigned char)
								* 8)))
				ft_error_msg("ft_ssl: Malloc failed");
			ft_memcpy(des->salt, buf + 8, 8);
			generate_keys_vector(des);
		}
	}
	*len = 0;
	return (0);
}

int				check_errors(unsigned char buf[], int len, int ret)
{
	(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
	if (buf[len - 1] > 8 ||
		ft_memcmp(buf + len - buf[len - 1],
				g_padding_patterns[buf[len - 1] - 1],
				buf[len - 1]))
	{
		ft_putendl_fd("ft_ssl: Bad decrypt", STDERR_FILENO);
		return (1);
	}
	else if (len % DES_BLOCK_SIZE)
	{
		ft_putendl_fd("ft_ssl: bad block length", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int		des_decrypt_message_reg(t_des *des)
{
	int				ret;
	int				len;
	unsigned char	buf[BASE64_BUF_SIZE + 1];

	if (init_decryption_with_salt(des, buf, &len))
		return (1);
	while ((ret = read(des->fd[IN], des->in, BUF_SIZE)) > 0)
	{
		(len + ret <= BUF_SIZE) ? ft_memcpy(buf + len, des->in, ret) :
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
	des_get_cipher(des, len, buf);
	if (check_errors(buf, len, ret))
		return (1);
	write(des->fd[OUT], buf, len - buf[len - 1]);
	return (0);
}

int				des_decrypt_message(t_des *des)
{
	if (des->opts & DES_OPT_A)
		return (des_decrypt_message_base64(des));
	return (des_decrypt_message_reg(des));
}

unsigned char	*des_decrypt_data(t_des *des, unsigned char *data,
									uint32_t data_len)
{
	unsigned char	*data_decrypted;
	uint64_t		cipher;
	uint64_t		plain;
	uint32_t		offset;

	offset = 0;
	data_decrypted = (unsigned char*)ft_malloc(data_len *
												sizeof(unsigned char));
	if (data_decrypted == NULL)
		return (NULL);
	ft_memset(data_decrypted, 0, data_len);
	while (offset < data_len)
	{
		cipher = convert_input_to_block(data + offset);
		plain = des->des_mode[1](cipher, des);
		cipher_to_string(plain, data_decrypted + offset);
		offset += DES_BLOCK_SIZE;
	}
	return (data_decrypted);
}
