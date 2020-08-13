/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_decrypt2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 11:52:39 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/11 11:52:40 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static int	init_decryption64_with_salt(t_des *des,
										unsigned char buf[],
										int *length)
{
	if (!(des->opts & DES_OPT_K))
	{
		if (*length < 16)
		{
			ft_putendl_fd("ft_ssl: error reading input file", STDERR_FILENO);
			return (1);
		}
		if (!ft_strncmp((char*)buf, "Salted__", 8))
		{
			ft_memdel((void**)&des->salt);
			if (!(des->salt = (unsigned char*)ft_malloc(sizeof(unsigned char)
								* 8)))
				ft_error_msg("ft_ssl: Malloc failed");
			ft_memcpy(des->salt, buf + 8, 8);
			generate_keys_vector(des);
			ft_memmove(buf, buf + 16, *length - 16);
			*length -= 16;
		}
	}
	return (0);
}

static int	write_b64_decoding(t_des *des,
								unsigned char buf[],
								int len, int final)
{
	int	length;

	length = base64_decode(buf, len, des->fd[OUT], 1);
	if (init_decryption64_with_salt(des, buf, &length))
		return (1);
	if (length % DES_BLOCK_SIZE)
	{
		ft_putendl_fd("ft_ssl: bad block lenght", STDERR_FILENO);
		return (1);
	}
	des_get_cipher(des, length, buf);
	if (final)
	{
		if (check_errors(buf, length, 0))
			return (1);
		write(des->fd[OUT], buf, length - buf[length - 1]);
	}
	else
		write(des->fd[OUT], buf, length);
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

int			des_decrypt_message_base64(t_des *des)
{
	int				ret;
	int				len;
	unsigned char	buf[BASE64_BUF_SIZE + 1];

	len = 0;
	while ((ret = read(des->fd[IN], des->in, BASE64_BUF_SIZE)) > 0)
	{
		ret = remove_newlines_from_input(des->in, ret);
		(len + ret > BASE64_BUF_SIZE) ? ft_memcpy(buf + len, des->in, ret) :
						ft_memcpy(buf + len, des->in, BASE64_BUF_SIZE - len);
		len += ret;
		if (len >= BASE64_BUF_SIZE)
		{
			if (write_b64_decoding(des, buf, BASE64_BUF_SIZE, 0))
				return (1);
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
