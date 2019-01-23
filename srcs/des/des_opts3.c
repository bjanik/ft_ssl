/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_opts3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/16 14:46:01 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 11:41:53 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int		set_encrypt_mode(char **argv, t_des *des, int *index)
{
	(void)argv;
	(void)index;
	des->opts &= ~DES_OPT_D;
	return (0);
}

int		set_password(char **argv, t_des *des, int *index)
{
	if (!argv[++(*index)])
	{
		ft_putendl_fd("ft_ssl: Password must be defined", STDERR_FILENO);
		return (1);
	}
	if (!(des->password = ft_strdup(argv[*index])))
		ft_error_msg("Malloc failed");
	return (0);
}

int		set_cap_p(char **argv, t_des *des, int *index)
{
	(void)argv;
	(void)index;
	des->opts |= DES_OPT_CAP_P;
	return (0);
}

int		set_base64(char **argv, t_des *des, int *index)
{
	(void)argv;
	(void)index;
	des->opts |= DES_OPT_A;
	if (!(des->base64 = ft_memalloc(sizeof(t_base64))))
		ft_error_msg("Malloc failed");
	return (0);
}

int		set_salt(char **argv, t_des *des, int *index)
{
	uint64_t	salt;

	if (!argv[++(*index)])
	{
		ft_putendl_fd("ft_ssl: missing argument salt for -s", STDERR_FILENO);
		return (1);
	}
	if (get_hex_from_str(argv[*index], &salt))
		return (1);
	des->salt = (unsigned char*)malloc(sizeof(unsigned char) * 8);
	ft_memset(des->salt, 0, 8);
	cipher_to_string(salt, des->salt);
	return (0);
}
