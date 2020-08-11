/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_opts3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:29:54 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:29:56 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int	rsa_usage(char *opt)
{
	int fd;

	fd = STDERR_FILENO;
	ft_dprintf(fd, "ft_ssl: rsa: invalid option %s\n", opt);
	ft_dprintf(fd, "Options are:\n");
	ft_dprintf(fd, "-in [file]  input file\n");
	ft_dprintf(fd, "-out [file] output file\n");
	ft_dprintf(fd, "-modulus    print the RSA key modulus\n");
	ft_dprintf(fd, "-check      check consistency of RSA private key\n");
	ft_dprintf(fd, "-noout 		do not print encoded version of the key\n");
	ft_dprintf(fd, "-text 		print in plain textin addtion to encoded\n");
	ft_dprintf(fd, "-pubin.     expect a public key as input\n");
	ft_dprintf(fd, "-pubout     output a public key\n");
	ft_dprintf(fd, "-passin     input file passphrase source\n");
	ft_dprintf(fd, "-passout    output file passphrase source\n");
	ft_dprintf(fd, "-des        encrypt the generated key with des-cbc\n");
	return (1);
}

int	set_pubin(char **argv, void *ptr, int *index)
{
	t_rsa	*rsa;

	(void)argv;
	(void)index;
	rsa = ptr;
	rsa->opts |= RSA_PUBIN;
	return (0);
}

int	set_pubout(char **argv, void *ptr, int *index)
{
	t_rsa	*rsa;

	(void)argv;
	(void)index;
	rsa = ptr;
	rsa->opts |= RSA_PUBOUT;
	return (0);
}
