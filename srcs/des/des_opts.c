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

static void	set_subkeys(t_des *des, char *str_key)
{
	uint64_t	key;

	get_hex_from_str(str_key, &key);
	key = get_56bits_key(key);
	get_subkeys(key >> 28, (key << 36) >> 36, des->keys);
}

void		set_in_out_files(t_des *des)
{
	if (des->in)
	{
		if ((des->fd[IN] = open(des->in, O_RDONLY, 0644)) < 0)
		{
			write(STDERR_FILENO, "ft_ssl: ", 8);
			perror(des->in);
			exit(EXIT_FAILURE);
		}
	}
	if (des->out)
	{
		if ((des->fd[OUT] = 
					open(des->out, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
		{
			write(STDERR_FILENO, "ft_ssl: ", 8);
			perror(des->out);
			exit(EXIT_FAILURE);
		}
	}
}

int			des_opts(char **argv, t_des *des)
{
	int	i;

	i = 1;
	while (argv[++i])
	{
		if (!ft_strcmp(argv[i], "-i"))
			des->in = argv[++i];
		else if (!ft_strcmp(argv[i], "-o"))
			des->out = argv[++i];
		else if (!ft_strcmp(argv[i], "-d"))
			des->opts |= DES_OPT_D;
		else if (!ft_strcmp(argv[i], "-e") && (des->opts & DES_OPT_D))
			des->opts &= ~DES_OPT_D;
		else if (!ft_strcmp(argv[i], "-v"))
			get_hex_from_str(argv[++i], &des->init_vector);
		else if (!ft_strcmp(argv[i], "-k"))
			set_subkeys(des, argv[++i]);
		else if (!ft_strcmp(argv[i], "-a"))
			des->opts |= DES_OPT_A;
		else if (!ft_strcmp(argv[i], "-nopad"))
			des->opts |= DES_NOPAD;
		else
		{
			ft_putendl_fd("ft_ssl: des: invalid option", STDERR_FILENO);
			return (1);
		}
	}
	(des->opts & DES_OPT_D) ? swap_keys(des->keys) : 0;
	return (0);
}
