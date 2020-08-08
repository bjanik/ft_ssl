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
	ft_dprintf(STDERR_FILENO, "ft_ssl: illegal option -- %c", c);
	ft_dprintf(STDERR_FILENO, "\nusage: ft_ssl %s %s", cmd_name,
			   HASH_CMD_USAGE);
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
		ft_dprintf(STDERR_FILENO, "%s:  option requires an argument -- s\n",
				   argv[1]);
		ft_dprintf(STDERR_FILENO, "usage: ft_ssl %s %s", argv[1],
				   HASH_CMD_USAGE);
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

int			set_options(t_ssl_command *command, char **argv, int *index)
{
	int	ret;

	if ((ret = parse_opt(command, argv, index)))
		return (ret);
	if (command->msg->str || command->msg->fd > -1)
	{
		command->hash_func(command->msg, command->opts);
		reset_msg(command->msg);
	}
	return (0);
}
