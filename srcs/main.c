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

static int		hash_string(t_msg *msg, char **argv, int *i)
{
	char	*s;

	if (ft_strlen(argv[*i]) > 2)
		s = (&argv[*i][2]);
	else if (argv[*i + 1])
		s = argv[(*i)++ + 1];
	else
	{
		ft_printf("%s: option requires an argument -- s\n", argv[1]);
		ft_printf("%s: usage: %s %s", argv[1], argv[1], HASH_CMD_USAGE);
		return (1);
	}
	init_msg(msg, s, NULL);
	return (0);
}

static void		hash_files(char **argv, t_ssl_command *command)
{
	int 	i;

	i = 0;
	while (argv[i])
	{
		if (init_msg(command->msg, NULL, argv[i++]) != 0)
			continue ;
		command->cmd_func(command->msg, command->opts);
	}
}

static void		main_loop(t_ssl_command *command, char **argv)
{
	int i;

	i = 2;
	if (!ft_strncmp("-s", argv[i], 2))
		hash_string(command->msg, argv, &i);
	else if (!ft_strncmp("-p", argv[i], 2))
	{
		init_msg(command->msg, NULL, NULL);
		command->opts |= OPT_P;
	}
	else if (!ft_strncmp("-r", argv[i], 2))
		command->opts |= OPT_R;
	else if (!ft_strncmp("-q", argv[i], 2))
		command->opts |= OPT_Q;
	if (command->msg->msg || command->msg->fd > -1)
	{
		command->cmd_func(command->msg, command->opts);
		reset_msg(command->msg);
	}
}

int				main(int argc, char **argv)
{
	t_ssl_command	*command;
	int 			i;

	command = NULL;
	i = 0;
	if (argc == 1)
		return (usage());
	if (!(command = get_ssl_command(argv[1])))
		return (command_usage(argv[1]));
	if (argc == 2)
	{
		init_msg(command->msg, NULL, NULL);
		command->cmd_func(command->msg, command->opts);
	}
	while (argv[i] && ft_strcmp(END_OF_OPT, argv[i]) && argv[i][0] == '-')
	{
		main_loop(command, argv);
		i++;
	}
	hash_files(argv + i, command);
	ft_memdel((void**)&(command->msg));
	return (0);
}
