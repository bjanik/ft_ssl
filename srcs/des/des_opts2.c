/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_opts2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/16 13:00:40 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/16 18:52:16 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int		set_input_file(char **argv, t_des *des, int *index)
{
	if (argv[++(*index)])
	{
		if ((des->fd[IN] = open(argv[*index], O_RDONLY, 0644)) < 0)
		{
			write(STDERR_FILENO, "ft_ssl: ", 8);
			perror(argv[*index]);
			return (1);
		}
	}
	else
	{
		ft_putendl_fd("ft_ssl: Missing input file", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int		set_output_file(char **argv, t_des *des, int *index)
{
	if (argv[++(*index)])
	{
		if ((des->fd[OUT] =
			open(argv[*index], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
		{
			write(STDERR_FILENO, "ft_ssl: ", 8);
			perror(argv[*index]);
			return (1);
		}
	}
	else
	{
		ft_putendl_fd("ft_ssl: Missing output file", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int		set_init_vector(char **argv, t_des *des, int *index)
{
	if (!argv[++(*index)])
	{
		ft_putendl_fd("ft_ssl: init vector must be defined", STDERR_FILENO);
		return (1);
	}
	des->opts |= DES_OPT_V;
	get_hex_from_str(argv[*index], &des->init_vector);
	return (0);
}

int		set_nopad(char **argv, t_des *des, int *index)
{
	(void)argv;
	(void)index;
	des->opts |= DES_NOPAD;
	return (0);
}

int		set_decrypt_mode(char **argv, t_des *des, int *index)
{
	(void)argv;
	(void)index;
	des->opts |= DES_OPT_D;
	return (0);
}