/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:18:58 by bjanik            #+#    #+#             */
/*   Updated: 2018/10/31 19:45:46 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static int		hash_files(char **argv, t_ssl_command *command)
{
	int	i;
	int	ret;
	int	rett;

	i = 0;
	rett = 0;
	while (argv[i])
	{
		if ((ret = init_msg(command->msg, NULL, argv[i++])) != 0)
		{
			rett = ret;
			continue ;
		}
		command->cmd_func(command->msg, command->opts);
	}
	return (rett);
}

static int		set_options(t_ssl_command *command, char **argv, int *index)
{
	int ret;

	if ((ret = parse_opt(command, argv, index)))
		return (ret);
	if (command->msg->msg || command->msg->fd > -1)
	{
		command->cmd_func(command->msg, command->opts);
		reset_msg(command->msg);
	}
	return (0);
}

int				main(int argc, char **argv)
{
	t_ssl_command	*command;
	int 			i;

	i = 2;
	if (argc == 1)
		return (usage());
	if (!(command = get_ssl_command(argv[1])))
		return (commands_usage(argv[1]));
	while (argv[i] && ft_strcmp(END_OF_OPT, argv[i]) && argv[i][0] == '-')
	{
		if (set_options(command, argv, &i))
			return (1);
		i++;
	}
	(argv[i] && !ft_strcmp(END_OF_OPT, argv[i])) ? i++ : 0;
	if (!argv[i] && !(command->opts & (OPT_S | OPT_P)))
	{
		init_msg(command->msg, NULL, NULL);
		command->cmd_func(command->msg, command->opts);
		reset_msg(command->msg);
	}
	i = hash_files(argv + i, command);
	ft_memdel((void**)&(command->msg));
	return (i);
}
