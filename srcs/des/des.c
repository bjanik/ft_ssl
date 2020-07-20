/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/03 14:22:12 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 11:45:41 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void			cipher_to_string(uint64_t cipher, unsigned char output[])
{
	uint8_t	i;

	i = 0;
	while (i < 8)
	{
		output[i] = (unsigned char)(cipher >> (56 - i * 8)) & 0xFF;
		i++;
	}
}

void			des_get_cipher(t_des *des, int len, unsigned char buf[])
{
	uint64_t	plain;
	uint64_t	cipher;
	int			offset;

	offset = 0;
	if (!ft_strncmp((char*)buf, "Salted__", 8) && des->salt)
	{
		offset = 16;
		ft_memdel((void**)&des->salt);
	}
	while (offset < len)
	{
		plain = convert_input_to_block(buf + offset);
		cipher = des->des_mode[des->opts & DES_OPT_D](plain, des);
		cipher_to_string(cipher, buf + offset);
		offset += DES_BLOCK_SIZE;
	}
}

static void		des_final(t_des *des, int len, unsigned char buf[])
{
	int	length;

	length = len;
	if (!(des->opts & DES_NOPAD) || !len)
	{
		ft_memset(buf + len, 8 - len % 8, 8 - len % 8);
		length = len + 8 - len % 8;
	}
	des_get_cipher(des, length, buf);
	if (des->opts & DES_OPT_A)
		base64_encode(buf, length, des->fd[OUT]);
	else
		write(des->fd[OUT], buf, length);
}

static int		insert_salt(t_des *des, unsigned char buf[])
{
	if (des->salt)
	{
		ft_memcpy(buf, "Salted__", 8);
		ft_memcpy(buf + 8, des->salt, 8);
		return (16);
	}
	return (0);
}

int				des_encrypt_message(t_des *des)
{
	int				ret;
	int				len;
	unsigned char	buf[BASE64_BUF_SIZE + 1];

	len = insert_salt(des, buf);
	while ((ret = read(des->fd[IN], des->in, BUF_SIZE)) > 0)
	{
		(len + ret > BUF_SIZE) ? ft_memcpy(buf + len, des->in, ret) :
						ft_memcpy(buf + len, des->in, BUF_SIZE - len);
		len += ret;
		if (len >= BUF_SIZE)
		{
			des_get_cipher(des, BUF_SIZE, buf);
			!(des->opts & DES_OPT_A) ? write(des->fd[OUT], buf, BUF_SIZE) :
					base64_encode(buf, BUF_SIZE, des->fd[OUT]);
			if (len > BUF_SIZE)
				ft_memcpy(buf, des->in + ret - len + BUF_SIZE,
												len - BUF_SIZE);
			len = (len > BUF_SIZE) ? len - BUF_SIZE : 0;
		}
	}
	(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
	des_final(des, len, buf);
	des->opts & DES_OPT_A ? ft_putchar_fd('\n', des->fd[OUT]) : 0;
	return (0);
}

unsigned char 		*des_encrypt_data(t_des *des, unsigned char *data, uint32_t *data_len)
{
	unsigned char 	*data_encrypted, *data_with_padding;
	uint64_t		plain, cipher;
	int 			offset, data_encrypted_len, padding_len;

	offset = 0;
	padding_len = 8 - *data_len % 8;
	data_encrypted_len = *data_len + padding_len;
	if ((data_with_padding = (unsigned char *)malloc(data_encrypted_len)) == NULL)
		return (NULL);
	ft_memcpy(data_with_padding, data, *data_len);
	ft_memset(data_with_padding + *data_len, padding_len, padding_len);
	data_encrypted = (unsigned char *)malloc(data_encrypted_len);
	if (data_encrypted == NULL)
	{
		ft_memdel((void**)&data_with_padding);
		return (NULL);
	}
	while (offset < data_encrypted_len)
	{
		plain = convert_input_to_block(data_with_padding + offset);
		cipher = des->des_mode[0](plain, des);
		cipher_to_string(cipher, data_encrypted + offset);
		offset += DES_BLOCK_SIZE;
	}
	ft_memdel((void**)&data_with_padding);
	*data_len = data_encrypted_len;
	return (data_encrypted);
}
