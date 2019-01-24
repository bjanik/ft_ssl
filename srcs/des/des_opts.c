/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_opts.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/04 14:53:54 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 15:03:42 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

const t_des_opts	g_des_opts[] =
{
	{"-i", set_input_file},
	{"-o", set_output_file},
	{"-d", set_decrypt_mode},
	{"-e", set_encrypt_mode},
	{"-k", set_subkeys},
	{"-v", set_init_vector},
	{"-nopad", set_nopad},
	{"-P", set_cap_p},
	{"-p", set_password},
	{"-a", set_base64},
	{"-s", set_salt},
	{NULL, NULL}
};

static int	create_subkeys(t_des *des, char *tmp)
{
	uint64_t	key;
	int			len;
	int			i;

	i = 0;
	while (i < 3)
	{
		if (get_hex_from_str(tmp, &key))
			return (1);
		len = ft_strlen(tmp);
		key = get_56bits_key(key);
		get_subkeys(key >> 28, (key << 36) >> 36, des->keys[i]);
		if (len > MAX_KEY_LEN)
			tmp += MAX_KEY_LEN;
		else
			tmp += len;
		i++;
	}
	return (0);
}

int			set_subkeys(char **argv, t_des *des, int *index)
{
	uint8_t		i;
	uint8_t		len;
	char		*tmp;

	if (!argv[++(*index)])
	{
		ft_putendl_fd("ft_ssl: Key is undefined", STDERR_FILENO);
		return (1);
	}
	des->opts |= DES_OPT_K;
	i = 0;
	tmp = argv[*index];
	if ((len = ft_strlen(argv[*index])) > 48)
		ft_memcpy(des->hex_keys, argv[*index], 48);
	else
		ft_memcpy(des->hex_keys, argv[*index], len);
	create_subkeys(des, tmp);
	swap_keys(des->keys[1]);
	return (0);
}

static int	finalize_opts(t_des *des)
{
	if ((ft_strcmp(des->name, "des-ecb") && ft_strcmp(des->name, "des3-ecb"))
			&& (des->opts & DES_OPT_K) && !(des->opts & DES_OPT_V))
	{
		ft_putendl_fd("ft_ssl: init vector undefined", STDERR_FILENO);
		return (1);
	}
	if (des->opts & DES_OPT_A)
		init_b64_fds(des, des->base64);
	if (des->opts & DES_OPT_D)
	{
		swap_keys(des->keys[0]);
		swap_keys(des->keys[1]);
		swap_keys(des->keys[2]);
	}
	return (0);
}

int			des_opts(char **argv, t_des *des)
{
	int	i;
	int j;

	i = 1;
	while (argv[++i])
	{
		j = -1;
		while (g_des_opts[++j].opt)
		{
			if (!ft_strcmp(argv[i], g_des_opts[j].opt))
			{
				if (g_des_opts[j].opt_f(argv, des, &i))
					return (1);
				break ;
			}
		}
		if (!g_des_opts[j].opt)
		{
			// ft_putstr_fd("ft_ssl: invalid option: ", STDERR_FILENO);
			// ft_putstr_fd(argv[i], STDERR_FILENO);
			return (des_usage(des->name, argv[i]));
		}
	}
	return (finalize_opts(des));
}
