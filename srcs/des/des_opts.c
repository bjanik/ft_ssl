/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_opts.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/04 14:53:54 by bjanik            #+#    #+#             */
/*   Updated: 2018/12/04 14:53:55 by bjanik           ###   ########.fr       */
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
	// {"-p", set_password},
	{NULL, NULL}
};
	// {"-a", set_b64},
	// {"-p", set_password}

void		set_subkeys(char **argv, t_des *des, int *index)
{
	uint64_t	key;
	uint8_t		i;
	uint8_t		len;

	if (!argv[++(*index)])
		ft_error_msg("ft_ssl: Key is undefined");
	i = 0;
	while (i < 3)
	{
		get_hex_from_str(argv[(*index)], &key);
		len = ft_strlen(argv[(*index)]);
		key = get_56bits_key(key);
		get_subkeys(key >> 28, (key << 36) >> 36, des->keys[i]);
		if (len > MAX_KEY_LEN)
			argv[(*index)] += MAX_KEY_LEN;
		else
			argv[(*index)] += len;
		i++;
	}
	swap_keys(des->keys[1]);
}

void		set_input_file(char **argv, t_des *des, int *index)
{
	if (argv[++(*index)])
	{
		if ((des->fd[IN] = open(argv[*index], O_RDONLY, 0644)) < 0)
		{
			write(STDERR_FILENO, "ft_ssl: ", 8);
			perror(argv[*index]);
			exit(EXIT_FAILURE);
		}
	}
	else
		ft_error_msg("ft_ssl: Missing input file");
}

void		set_output_file(char **argv, t_des *des, int *index)
{
	if (argv[++(*index)])
	{
		if ((des->fd[OUT] = 
					open(argv[*index], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
		{
			write(STDERR_FILENO, "ft_ssl: ", 8);
			perror(argv[*index]);
			exit(EXIT_FAILURE);
		}
	}
	else
		ft_error_msg("ft_ssl: Missing output file");
}

void				set_init_vector(char **argv, t_des *des, int *index)
{
	if (!argv[++(*index)])
		ft_error_msg("ft_ssl: initialization vector must be defined");
	des->opts |= DES_OPT_V;
	get_hex_from_str(argv[*index], &des->init_vector);
}


inline void	set_nopad(char **argv, t_des *des, int *index)
{
	(void)argv;
	(void)index;
	des->opts |= DES_NOPAD;
}

inline void	set_decrypt_mode(char **argv, t_des *des, int *index)
{
	(void)argv;
	(void)index;
	des->opts |= DES_OPT_D;
}

inline void	set_encrypt_mode(char **argv, t_des *des, int *index)
{
	(void)argv;
	(void)index;
	des->opts &= ~DES_OPT_D;
}

void		set_password(char **argv, t_des *des, int *index)
{
	if (!argv[++(*index)])
		ft_error_msg("ft_ssl: Password must be defined");
	if (!(des->password = ft_strdup(argv[*index])))
		ft_error_msg("ft_ssl: Malloc failed");
}


inline void	set_cap_p(char **argv, t_des *des, int *index)
{
	(void)argv;
	(void)index;
	des->opts |= DES_OPT_CAP_P;
}

int			des_opts(char **argv, t_des *des)
{
	int	i;
	int j;

	i = 1;
	while (argv[++i])
	{
		j = 0;
		while (g_des_opts[j].opt)
		{
			if (!ft_strcmp(argv[i], g_des_opts[j].opt))
				g_des_opts[j].opt_f(argv, des, &i);
			j++;
		}
	}
	if ((ft_strcmp(des->name, "des-ecb") && ft_strcmp(des->name, "des3-ecb"))
			&& !(des->opts & DES_OPT_V))
		ft_error_msg("ft_ssl: initialization vector undefined");
	if (des->opts & DES_OPT_D)
	{
		swap_keys(des->keys[0]);
		swap_keys(des->keys[1]);
		swap_keys(des->keys[2]);
	}
	return (0);
}
		// if (!ft_strcmp(argv[i], "-i"))
		// 	des->in = argv[++i];
		// else if (!ft_strcmp(argv[i], "-o"))
		// 	des->out = argv[++i];
		// else if (!ft_strcmp(argv[i], "-d"))
		// 	des->opts |= DES_OPT_D;
		// else if (!ft_strcmp(argv[i], "-e"))
		// 	des->opts &= ~DES_OPT_D;
		// else if (!ft_strcmp(argv[i], "-v"))
		// {
		// 	des->opts |= DES_OPT_V;
		// 	!argv[i + 1] ? ft_error_msg("ft_ssl: iv undefined") : 0;
		// 	get_hex_from_str(argv[++i], &des->init_vector);
		// }
		// else if (!ft_strcmp(argv[i], "-k"))
		// 	set_subkeys(des, argv[++i]);
		// else if (!ft_strcmp(argv[i], "-a"))
		// 	des->opts |= DES_OPT_A;
		// else if (!ft_strcmp(argv[i], "-nopad"))
		// 	des->opts |= DES_NOPAD;
		// else if (!ft_strcmp(argv[i], "-p"))
		// 	des->password = argv[++i];
		// else
		// 	ft_error_msg("ft_ssl: des: invalid option");
	
