/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pbkdf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 15:43:54 by bjanik            #+#    #+#             */
/*   Updated: 2018/12/07 15:43:56 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

unsigned char	*pbkdf(char *password, unsigned char *salt)
{
	unsigned char		*hash;
	unsigned char		*ps;
	t_msg				msg;
	t_ctx				ctx;

	if (!password)
		return (NULL);
	if (!(ps = ft_memalloc(ft_strlen(password) + 9)))
		return (NULL);
	ft_memcpy(ps, password, ft_strlen(password));
	ft_memcpy(ps + ft_strlen(password), salt, 8);
	msg.str = (char*)ps;
	msg.msg_len = 32;
	sha256_init(&ctx);
	hash = sha256_core(&ctx, &msg, 0);
	ft_strdel((char**)&ps);
	msg.str = NULL;
	return (hash);
}

uint64_t	get_keys_vector_from_hash(unsigned char *keys)
{
	uint64_t	tmp;
	uint64_t	key;
	uint8_t		i;

	i = 0;
	key = 0;
	while (i < 8)
	{
		tmp = keys[i];
		key |= tmp << (56 - i * 8);
		i++;
	}
	return (key);
}


void	display_skv(t_des *des, unsigned char *salt,
					unsigned char *keys[],
					unsigned char *iv)
{
	int 		i;
	uint8_t 	nb_keys;

	i = -1;
	nb_keys = (ft_strchr(des->name, '3')) ? 3 : 1;
	if (des->opts & DES_OPT_CAP_P)
	{
		ft_printf("salt=");
		print_hash((unsigned char*)salt, 8, 1);
		write(STDOUT_FILENO, "\n", 1);
		ft_printf("key=");
		while (++i < nb_keys)
			print_hash(keys[i], 8, 1);
		write(STDOUT_FILENO, "\n", 1);
		if (!ft_strstr(des->name, "ecb"))
		{
			ft_printf("iv=");
			print_hash(iv, 8, 1);
			write(STDOUT_FILENO, "\n", 1);	
		}
		exit(EXIT_SUCCESS);
	}
}

static unsigned char	*set_key(t_des *des, unsigned char *hash, int i)
{
	uint64_t		key;
	unsigned char	*ks;

	if (!(ks = (unsigned char*)ft_strndup((char*)hash, 8)))
		ft_error_msg("ft_ssl: Malloc failed");
	key = get_keys_vector_from_hash(ks);
	key = get_56bits_key(key);
	get_subkeys(key >> 28, (key << 36) >> 36, des->keys[i]);
	return (ks);
}

void	generate_keys_vector(t_des *des)
{
	unsigned char 	*hash;
	unsigned char	*keys[3];
	unsigned char	*iv;

	(!des->password) ? des->password = get_password(des->opts & DES_OPT_D) : 0;
	get_salt(des);
	hash = pbkdf(des->password, des->salt);
	keys[i] = set_key(des, hash, i);
	// if (!(ks[i] = (unsigned char*)ft_strndup((char*)hash, 8)))
	// 	ft_error_msg("ft_ssl: Malloc failed");
	// keys[i] = get_keys_vector_from_hash(ks[i]);
	// keys[i] = get_56bits_key(keys[i]);
	// get_subkeys(keys[i] >> 28, (keys[i] << 36) >> 36, des->keys[i]);
	if (!(des->opts & DES_OPT_V))
	{
		if (!(iv = (unsigned char*)ft_strdup((char*)hash + 8)))
			ft_error_msg("ft_ssl: Malloc failed");
		des->init_vector = get_keys_vector_from_hash(iv);
	}
	display_skv(des, des->salt[0], keys, iv);
	ft_strdel((char**)&iv);
	write(des->fd[OUT], "Salted__", 8);
	write(des->fd[OUT], salt[0], 8);
	ft_strdel((char**)&salt[0]);
}
