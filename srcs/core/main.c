/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:18:58 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 12:36:58 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"
#include "ft_ssl.h"

int	hash_files(char **argv, int opts, t_ssl_command *command)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (argv[i])
		ret = command->hash_func(opts, NULL, argv[i++]);
	return (ret);
}

int	ft_ssl_routine(char **argv)
{
	t_ssl_command	*command;

	if (!argv[1])
		return (1);
	if (!ft_strcmp("exit", argv[1]) || !ft_strcmp("quit", argv[1]))
		return (0);
	if (!(command = get_ssl_command(argv)))
		return (commands_usage(argv[1]));
	return (command->func(argv, command));
}

int	main(int argc, char **argv)
{
	int			ret;

	if (argc == 1)
		ret = interactive_mode(argv);
	else
		ret = ft_ssl_routine(argv);
	return (ret);
}
