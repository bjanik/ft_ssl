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

/*
** Return the first 8 bytes of hash to produce key
*/

char	*get_salt(void)
{
	int 	fd;
	char	*salt;

	if (!(salt = ft_memalloc(sizeof(char) * 9)))
		ft_error_msg("ft_ssl: malloc failed");
	ft_memset(salt, 0x0, 9);
	if ((fd = open("/dev/random", O_RDONLY, 0644)) < 0)
		ft_error_msg("ft_ssl: /dev/random opening failed");
	if (read(fd, salt, 8) < 0)
	{
		close(fd);	
	 	ft_error_msg("ft_ssl: read failed");
	}
	// write(1, salt, 8);
	// write(1, "\n", 1);
	close(fd);
	return (salt);
}

unsigned char	*pbkdf(char *password, char *salt)
{
	unsigned char		*hash;
	unsigned char		*ps;
	t_msg				msg;
	t_ctx				ctx;

	if (!(ps = ft_memalloc(ft_strlen(password) + 9)))
		return (NULL);
	ft_memcpy(ps, password, ft_strlen(password));
	ft_memcpy(ps + ft_strlen(password), salt, 8);
	msg.str = (char*)ps;
	msg.msg_len = 16;
	md5_init(&ctx);
	hash = md5_core(&ctx, &msg, 0);
	ft_strdel((char**)&ps);
	return (hash);
}

static void	get_keys_vector_from_hash(uint64_t key_iv[], unsigned char *keys[])
{
	uint64_t	tmp;
	uint8_t		i;

	i = 0;
	key_iv[0] = 0;
	key_iv[1] = 0;
	while (i < 8)
	{
		tmp = keys[0][i];
		key_iv[0] |= tmp << (56 - i * 8);	
	}
	i = 0;
	while (i < 8)
	{
		tmp = keys[1][i];
		key_iv[1] |= tmp << (56 - i * 8);
	}
}

void	generate_keys_vector(t_des *des)
{
	char			*pwd[2];
	char			*salt;
	unsigned char 	*hash;
	uint64_t		key_iv[2];
	unsigned char	*keys[2];

	if (des->opts & DES_OPT_D)
	{
		pwd[0] = ft_strdup(getpass("Enter des decryption password:"));
		pwd[1] = ft_strdup(getpass("Verifying des decryption password:"));
	}
	else
	{
		pwd[0] = ft_strdup(getpass("Enter des encryption password:"));
		pwd[1] = ft_strdup(getpass("Verifying des encryption password:"));
	}
	(!pwd[0] || !pwd[1]) ? ft_error_msg("ft_ssl: bad password read") : 0;
	(ft_strcmp(pwd[0], pwd[1])) ? ft_error_msg("Verify failure") : 0;
	salt = get_salt();
	hash = pbkdf(pwd[0], salt);
	keys[0] = (unsigned char*)ft_strndup((char*)hash, 8);
	keys[1] = (unsigned char*)ft_strdup((char*)hash + 8);
	get_keys_vector_from_hash(key_iv, keys);
	des->keys[0] = key_iv[0];
	des->init_vector = key_iv[1];
}
