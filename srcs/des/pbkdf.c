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
	close(fd);
	return (salt);
}

unsigned char	*pbkdf(char *password, char *salt)
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
	msg.msg_len = 16;
	md5_init(&ctx);
	hash = md5_core(&ctx, &msg, 0);
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

char	*get_password(int encryption)
{
	char	*pwd;
	char	*check_pwd;

	if (encryption)
	{
		pwd = ft_strdup(getpass("Enter DES decryption password:"));
		check_pwd = getpass("Verifying DES decryption password:");
	}
	else
	{
		pwd = ft_strdup(getpass("Enter DES encryption password:"));
		check_pwd = getpass("Verifying DES encryption password:");
	}
	(!pwd || !check_pwd) ? ft_error_msg("ft_ssl: bad password read") : 0;
	(ft_strcmp(pwd, check_pwd)) ? ft_error_msg("Verify failure") : 0;
	return (pwd);
}

void	display_skv(t_des *des, char *salt,
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

void	generate_keys_vector(t_des *des)
{
	char			*salt[3];
	unsigned char 	*hash;
	uint64_t		keys[3];
	unsigned char	*ks[3];
	unsigned char	*iv;
	int				i;

	(!des->password) ? des->password = get_password(des->opts & DES_OPT_D) : 0;
	i = -1;
	while (++i < 3)
	{
		salt[i] = get_salt();
		hash = pbkdf(des->password, salt[i]);
		if (!(ks[i] = (unsigned char*)ft_strndup((char*)hash, 8)))
			ft_error_msg("ft_ssl: Malloc failed");
		keys[i] = get_keys_vector_from_hash(ks[i]);
		keys[i] = get_56bits_key(keys[i]);
		get_subkeys(keys[i] >> 28, (keys[i] << 36) >> 36, des->keys[i]);
		if (!i && !(des->opts & DES_OPT_V))
		{
			if (!(iv = (unsigned char*)ft_strdup((char*)hash + 8)))
				ft_error_msg("ft_ssl: Malloc failed");
			des->init_vector = get_keys_vector_from_hash(iv);
		}
		i ? ft_strdel(&salt[i]) : 0;
		ft_strdel((char**)&hash);
	}
	display_skv(des, salt[0], ks, iv);
	ft_strdel((char**)&iv);
	write(des->fd[OUT], "Salted__", 8);
	write(des->fd[OUT], salt[0], 8);
	ft_strdel(&salt[0]);
}
