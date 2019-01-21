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

#define SINGLE_DES 1
#define TRIPLE_DES 3

static unsigned char	*pbkdf(char *password, unsigned char *salt, int des)
{
	unsigned char		*hash;
	unsigned char		*ps;
	t_msg				msg;
	t_ctx				ctx;

	if (!password || !(ps = ft_memalloc(ft_strlen(password) + 9)))
		return (NULL);
	ft_memcpy(ps, password, ft_strlen(password));
	ft_memcpy(ps + ft_strlen(password), salt, 8);
	msg.str = ps;
	msg.msg_len = ft_strlen(password) + 8;
	if (des == SINGLE_DES)
	{
		md5_init(&ctx);
		hash = md5_core(&ctx, &msg, 0);
	}
	else
	{
		sha256_init(&ctx);
		hash = sha256_core(&ctx, &msg, 0);
	}
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


int	display_skv(t_des *des, unsigned char *salt,
					unsigned char keys[][8],
					unsigned char iv[])
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
		return (1);
	}
	return (0);
}

static int	set_key(t_des *des,
								 unsigned char *hash,
								 unsigned char iv[],
								 unsigned char keys[][8])
{
	uint64_t		key;
	int 			nb;
	int 			i;

	nb = (ft_strchr(des->name, '3')) ? 3 : 1;
	i = 0;
	while (i < nb)
	{
		ft_memcpy(keys[i], hash + i * 8, 8);
		key = get_keys_vector_from_hash(keys[i]);
		key = get_56bits_key(key);
		get_subkeys(key >> 28, (key << 36) >> 36, des->keys[i]);
		i++;
	}
	if (!(des->opts & DES_OPT_V))
	{
		ft_memcpy(iv, hash + i * 8, 8);
		des->init_vector = get_keys_vector_from_hash(iv);
	}
	return (0);
}

int	generate_keys_vector(t_des *des)
{
	unsigned char 	*hash;
	unsigned char	keys[3][8];
	unsigned char	iv[8];

	if (!des->password)
		if (!(des->password = get_password(des->opts & DES_OPT_D)))
			return (1);
	!des->salt ? get_salt(des) : 0;
	if (ft_strchr(des->name, '3'))
		hash = pbkdf(des->password, des->salt, TRIPLE_DES);
	else
		hash = pbkdf(des->password, des->salt, SINGLE_DES);
	if (!hash)
		return (1);
	if (des->opts & DES_OPT_V)
		cipher_to_string(des->init_vector, iv);
	set_key(des, hash, iv, keys);
	ft_strdel((char**)&hash);
	if (display_skv(des, des->salt, keys, iv))
		return (1);
	(des->opts & DES_OPT_D) ? swap_keys(des->keys[0]) : 0;
	(des->opts & DES_OPT_D) ? swap_keys(des->keys[1]) : 0;
	(des->opts & DES_OPT_D) ? swap_keys(des->keys[2]) : 0;
	return (0);
}
