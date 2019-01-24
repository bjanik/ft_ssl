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

int	des_usage(const char *name, char *opt)
{
	ft_putstr_fd("ft_ssl: ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(": invalid option: ", STDERR_FILENO);
	ft_putendl_fd(opt, STDERR_FILENO);
	ft_putendl_fd("Options are:", STDERR_FILENO);
	ft_putendl_fd("-i [file] input file", STDERR_FILENO);
	ft_putendl_fd("-o [file] output file", STDERR_FILENO);
	ft_putendl_fd("-e        encrypt", STDERR_FILENO);
	ft_putendl_fd("-d        decrypt", STDERR_FILENO);
	ft_putendl_fd("-a        base64 encode/decode", STDERR_FILENO);
	ft_putendl_fd("-k        key is the next argument", STDERR_FILENO);
	ft_putendl_fd("-p        password is the next argument", STDERR_FILENO);
	ft_putendl_fd("-P        output keys/iv/salt values", STDERR_FILENO);
	ft_putendl_fd("-s        salt is the next argument", STDERR_FILENO);
	ft_putendl_fd("-v        init vector is the next argument", STDERR_FILENO);
	ft_putendl_fd("-nopad    disables padding for encryption", STDERR_FILENO);
	return (1);
}

int base64_usage(char *opt)
{
	ft_putstr_fd("ft_ssl: base64: invalid option: ", STDERR_FILENO);
	ft_putendl_fd(opt, STDERR_FILENO);
	ft_putendl_fd("Options are:", STDERR_FILENO);
	ft_putendl_fd("-i [file] input file", STDERR_FILENO);
	ft_putendl_fd("-o [file] output file", STDERR_FILENO);
	ft_putendl_fd("-e        encode", STDERR_FILENO);
	ft_putendl_fd("-d        decode", STDERR_FILENO);
	return (1);

}
