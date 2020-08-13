/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pbkdf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 15:43:54 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 11:59:04 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

unsigned char	*pbkdf(char *password, unsigned char *salt, int des)
{
	unsigned char		*hash;
	unsigned char		*ps;
	t_msg				msg;
	t_ctx				ctx;

	if (!password || !(ps = ft_memalloc(ft_strlen(password) + 9)))
		return (NULL);
	ft_memcpy(ps, password, ft_strlen(password));
	if (salt)
	{
		ft_memcpy(ps + ft_strlen(password), salt, 8);
		msg.msg_len = ft_strlen(password) + 8;
	}
	msg.str = ps;
	hash = gen_hash(&ctx, &msg, des);
	ft_strdel((char**)&ps);
	msg.str = NULL;
	return (hash);
}

uint64_t		get_keys_vector_from_hash(unsigned char *keys)
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

int				display_skv(t_des *des)
{
	int		i;
	uint8_t	nb_keys;

	i = -1;
	nb_keys = (ft_strchr(des->name, '3')) ? 3 : 1;
	ft_printf("salt=");
	print_hash((unsigned char*)des->salt, 8, 1);
	write(STDOUT_FILENO, "\n", 1);
	ft_printf("key=");
	while (++i < nb_keys)
		write(STDOUT_FILENO, des->hex_keys + i * 16, 16);
	write(STDOUT_FILENO, "\n", 1);
	if (!ft_strstr(des->name, "ecb"))
	{
		ft_printf("iv=");
		write(STDOUT_FILENO, des->hex_keys + nb_keys * 16, 16);
		write(STDOUT_FILENO, "\n", 1);
	}
	return (0);
}

void			set_key(t_des *des,
							unsigned char *hash,
							unsigned char keys[][8],
							int nb)
{
	uint64_t	key;
	int			i;
	char		*s;

	i = -1;
	while (++i < nb)
	{
		ft_memcpy(keys[i], hash + i * 8, 8);
		key = get_keys_vector_from_hash(keys[i]);
		s = ft_itoa_base_llu(key, "0123456789ABCDEF");
		ft_memcpy(des->hex_keys + i * 16, s, 16);
		ft_strdel(&s);
		key = get_56bits_key(key);
		get_subkeys(key >> 28, (key << 36) >> 36, des->keys[i]);
	}
	if (!(des->opts & DES_OPT_V))
	{
		ft_memcpy(keys[3], hash + i * 8, 8);
		if (des->init_vector == 0)
			des->init_vector = get_keys_vector_from_hash(keys[3]);
		s = ft_itoa_base_llu(des->init_vector, "0123456789ABCDEF");
		ft_memcpy(des->hex_keys + i * 16, s, 16);
		ft_strdel(&s);
	}
}

int				generate_keys_vector(t_des *des)
{
	unsigned char	*hash;
	unsigned char	keys[4][8];
	int				nb;

	if (!des->password)
		if (!(des->password = get_password(des->opts & DES_OPT_D)))
			return (1);
	!des->salt ? get_salt(des) : 0;
	nb = (ft_strchr(des->name, '3')) ? TRIPLE_DES : SINGLE_DES;
	if (!(hash = pbkdf(des->password, des->salt, nb)))
		return (1);
	if (des->opts & DES_OPT_V)
		cipher_to_string(des->init_vector, keys[3]);
	set_key(des, hash, keys, nb);
	des->opts |= DES_OPT_K;
	ft_strdel((char**)&hash);
	(des->opts & DES_OPT_D) ? swap_keys(des->keys[0]) : 0;
	(des->opts & DES_OPT_D) ? swap_keys(des->keys[1]) : 0;
	(des->opts & DES_OPT_D) ? swap_keys(des->keys[2]) : 0;
	return (0);
}
