/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usage.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 15:23:30 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 17:47:26 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int	usage(void)
{
	write(STDERR_FILENO, FT_SSL_USAGE, ft_strlen(FT_SSL_USAGE));
	return (1);
}

int	commands_usage(char *command)
{
	int	i;

	i = -1;
	ft_dprintf(STDERR_FILENO, "ft_ssl: %s is a invalid command.\n\n", command);
	ft_dprintf(STDERR_FILENO, "Standard commands:\ngenrsa\nrsa\nrsautl\n\n");
	ft_dprintf(STDERR_FILENO, "Message digest commands:\n");
	while (g_commands[++i].name)
		(g_commands[i].hash_func) ?
			ft_dprintf(STDERR_FILENO, "%s\n", g_commands[i].name) : 0;
	ft_dprintf(STDERR_FILENO, "\nCipher commands:\n");
	i = -1;
	while (g_commands[++i].name)
		(g_commands[i].des_mode[0]) ?
			ft_dprintf(STDERR_FILENO, "%s\n", g_commands[i].name) : 0;
	return (1);
}

int	des_usage(const char *name, char *opt)
{
	ft_dprintf(STDERR_FILENO, "ft_ssl: %s: invalid option: %s\n", name, opt);
	ft_dprintf(STDERR_FILENO, "Options are:\n");
	ft_dprintf(STDERR_FILENO, "-i [file] input file\n");
	ft_dprintf(STDERR_FILENO, "-o [file] output file\n");
	ft_dprintf(STDERR_FILENO, "-e        encrypt\n");
	ft_dprintf(STDERR_FILENO, "-d        decrypt\n");
	ft_dprintf(STDERR_FILENO, "-a        base64 encode/decode\n");
	ft_dprintf(STDERR_FILENO, "-k        key is the next argument\n");
	ft_dprintf(STDERR_FILENO, "-p        password is the next argument\n");
	ft_dprintf(STDERR_FILENO, "-P        output keys/iv/salt values\n");
	ft_dprintf(STDERR_FILENO, "-s        salt is the next argument\n");
	ft_dprintf(STDERR_FILENO, "-v        init vector is the next argument\n");
	ft_dprintf(STDERR_FILENO, "-nopad    disables padding for encryption\n");
	return (1);
}

int	base64_usage(char *opt)
{
	ft_dprintf(STDERR_FILENO, "ft_ssl: base64: invalid option: %s\n", opt);
	ft_dprintf(STDERR_FILENO, "Options are:\n");
	ft_dprintf(STDERR_FILENO, "-i [file] input file\n");
	ft_dprintf(STDERR_FILENO, "-o [file] output file\n");
	ft_dprintf(STDERR_FILENO, "-e        encode\n");
	ft_dprintf(STDERR_FILENO, "-d        decode\n");
	return (1);
}
