/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:18:58 by bjanik            #+#    #+#             */
/*   Updated: 2018/11/05 10:23:21 by bjanik           ###   ########.fr       */
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
		command->hash_func(command->msg, command->opts);
	}
	return (rett);
}

static int		set_options(t_ssl_command *command, char **argv, int *index)
{
	int ret;

	if ((ret = parse_opt(command, argv, index)))
		return (ret);
	if (command->msg->str || command->msg->fd > -1)
	{
		command->hash_func(command->msg, command->opts);
		reset_msg(command->msg);
	}
	return (0);
}

static int 		hash_algo(char **argv, t_ssl_command *command)
{
	int 	i;

	i = 2;
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
		command->hash_func(command->msg, command->opts);
		reset_msg(command->msg);
	}
	i = hash_files(argv + i, command);
	return (i);
}

static int 		data_encryption_standard(char **argv, t_ssl_command *command)
{
	if (des_opts(argv, command->des))
		return (1);
	set_in_out_files(command->des);
	des_message(command->des);
	return (0);
}

int				main(int argc, char **argv)
{
	t_ssl_command	*command;

	if (argc == 1)
		return (usage());
	if (!(command = get_ssl_command(argv[1])))
		return (commands_usage(argv[1]));
	if (command->hash_func)
		return (hash_algo(argv, command));
	else
		return (data_encryption_standard(argv, command));
	return (0);
}
