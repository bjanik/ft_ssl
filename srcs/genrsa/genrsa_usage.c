/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   genrsa_usage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 18:08:12 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/12 18:08:13 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int	genrsa_usage(char *opt)
{
	int fd;

	fd = STDERR_FILENO;
	ft_dprintf(fd, "ft_ssl: genrsa: invalid option %s\n", opt);
	ft_dprintf(fd, "Options are:\n");
	ft_dprintf(fd, "-o [file]      output the private key to file\n");
	ft_dprintf(fd, "-des           encrypt the generated key with des-cbc\n");
	ft_dprintf(fd, "-passout [arg] output file passphrase source\n");
	return (1);
}
