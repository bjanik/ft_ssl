/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:18:58 by bjanik            #+#    #+#             */
/*   Updated: 2018/10/29 11:24:26 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void			hash_string(t_msg *msg, char **argv, int *i)
{
	char	*s;

	if (ft_strlen(argv[*i]) > 2)
		s = (&argv[*i][2]);
	else if (argv[*i + 1])
		s = argv[(*i)++ + 1];
	else
	{
		ft_printf("md5: option requires an argument -- s\n");
		ft_printf("%s", MD5_USAGE);
		exit(1);
	}
	
	init_msg(msg, s, NULL);
}

void			main_loop(t_ssl_command *command, char **argv, int i)
{
	while (argv[i] && ft_strcmp(END_OF_OPT, argv[i]) && argv[i][0] == '-')
	{
		if (!ft_strncmp("-s", argv[i], 2))
			hash_string(command->msg, argv, &i);
		else if (!ft_strncmp("-p", argv[i], 2))
			init_msg(command->msg, NULL, NULL);
		else if (!ft_strncmp("-r", argv[i], 2))
			command->opts |= OPT_R;
		else if (!ft_strncmp("-q", argv[i], 2))
			command->opts |= OPT_Q;
		if (command->msg->msg || command->msg->fd > -1)
		{
			command->cmd_func(command->msg, command->opts);
			reset_msg(command->msg);
		}
		i++;
	}
	while (argv[i])
	{
		if (init_msg(command->msg, NULL, argv[i++]) != 0)
			continue ; 
		command->cmd_func(command->msg, command->opts);
	}
}

int		main(int argc, char **argv)
{
	t_ssl_command	*command;

	if (argc == 1)
		usage();
	if (!(command = get_ssl_command(argv[1])))
		command_usage(argv[1]);
	if (argc == 2)
	{
		init_msg(command->msg, NULL, NULL);	
		command->cmd_func(command->msg, command->opts);
	}
	main_loop(command, argv, 2);
}
