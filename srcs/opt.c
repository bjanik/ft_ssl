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

int	illegal_opt(char c, char *cmd_name)
{
	write(STDERR_FILENO, "ft_ssl: illegal option -- ", 26);
	write(STDERR_FILENO, &c, 1);
 	write(STDERR_FILENO, cmd_name, ft_strlen(cmd_name));
 	return (-1);
}


int	parse_opt(t_ssl_command *command, char *opt, char *cmd_name, int *index)
{
	int	i;

	i = 0;
	while (opt[++i])
	{
		// if (!ft_strchr(HASH_CMD_OPTS, opt[i]))
		// 	return (illegal_opt(opt[i], cmd_name));
		if (opt[i] == 'p')
		{
			command->opts |= OPT_P;
			init_msg(command->msg, NULL, NULL);
		}
		else if (opt[i] == 'r')
			command->opts |= OPT_R;
		else if (opt[i] == 'q')
			command->opts |= OPT_Q;
		else if (opt[i] == 's')
			hash_string(command->msg, argv, &index);
		else
			return (illegal_opt(opt[i], cmd_name));
	}
	return (0);
}
