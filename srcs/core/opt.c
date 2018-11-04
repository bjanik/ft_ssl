/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 16:31:13 by bjanik            #+#    #+#             */
/*   Updated: 2018/10/31 16:31:14 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int			illegal_opt(char c, char *cmd_name)
{
	write(STDERR_FILENO, "ft_ssl: illegal option -- ", 26);
	write(STDERR_FILENO, &c, 1);
	write(STDERR_FILENO, "\nusage: ft_ssl ", 15);
 	write(STDERR_FILENO, cmd_name, ft_strlen(cmd_name));
 	write(STDERR_FILENO, HASH_CMD_USAGE, ft_strlen(HASH_CMD_USAGE));
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
	init_msg(msg, s, NULL);
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
			command->cmd_func(command->msg, command->opts);
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
