/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 16:31:13 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 11:33:06 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int			illegal_opt(char c, char *cmd_name)
{
	ft_putstr_fd("ft_ssl: illegal option -- ", STDERR_FILENO);
	ft_putchar_fd(c, STDERR_FILENO);
	ft_putstr_fd("\nusage: ft_ssl ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putstr_fd(HASH_CMD_USAGE, STDERR_FILENO);
	return (1);
}

static int	hash_string(t_msg *msg, char **argv, int *ind, int i)
{
	char	*s;

	if (argv[*ind][i + 1])
		s = &argv[*ind][i + 1];
	else if (argv[*ind + 1])
		s = argv[(*ind)++ + 1];
	else
	{
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": option requires an argument -- s\n", STDERR_FILENO);
		ft_putstr_fd("usage: ft_ssl ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(HASH_CMD_USAGE, STDERR_FILENO);
		return (1);
	}
	init_msg(msg, (unsigned char*)s, NULL);
	return (0);
}

int			parse_opt(t_ssl_command *command, char **argv, int *index)
{
	int		i;

	i = 0;
	while (argv[*index][++i])
	{
		if (argv[*index][i] == 'p')
		{
			command->opts |= OPT_P;
			init_msg(command->msg, NULL, NULL);
			command->hash_func(command->msg, command->opts);
			reset_msg(command->msg);
		}
		else if (argv[*index][i] == 'r')
			command->opts |= OPT_R;
		else if (argv[*index][i] == 'q')
			command->opts |= OPT_Q;
		else if (argv[*index][i] == 's')
		{
			command->opts |= OPT_S;
			return (hash_string(command->msg, argv, index, i));
		}
		else
			return (illegal_opt(argv[*index][i], argv[1]));
	}
	return (0);
}
