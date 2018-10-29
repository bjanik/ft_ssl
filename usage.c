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

void	usage(void)
{
	write(STDERR_FILENO, FT_SSL_USAGE, strlen(FT_SSL_USAGE));
	exit(EXIT_FAILURE);
}

void	command_usage(char *command)
{
	printf("ft_ssl: Error: '%s' is an invalid command\n\n", command);
	printf("Standard commands:\n\n");
	printf("Message digest commands:\nmd5\nsha256\n\n");
	printf("Cipher commands:\n");
	exit(EXIT_FAILURE);
}
