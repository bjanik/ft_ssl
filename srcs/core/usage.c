/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usage.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 15:23:30 by bjanik            #+#    #+#             */
/*   Updated: 2018/10/29 15:23:32 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int	usage(void)
{
	write(STDERR_FILENO, FT_SSL_USAGE, strlen(FT_SSL_USAGE));
	return (1);
}

int	commands_usage(char *command)
{
	int	i;

	i = -1;
	ft_putstr_fd("ft_ssl: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(" is an invalid command\n", STDERR_FILENO);
	ft_putendl_fd("Standard commands:", STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	ft_putendl_fd("Message digest commands:", STDERR_FILENO);
	while (g_commands[++i].name)
		(g_commands[i].hash_func) ? 
			ft_putendl_fd(g_commands[i].name, STDERR_FILENO) : 0;
	ft_putchar_fd('\n', STDERR_FILENO);
	ft_putendl_fd("Cipher commands:", STDERR_FILENO);
	i = -1;
	while (g_commands[++i].name)
		(!g_commands[i].hash_func) ? 
			ft_putendl_fd(g_commands[i].name, STDERR_FILENO) : 0;
	return (1);
}
