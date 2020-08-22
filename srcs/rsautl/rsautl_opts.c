/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsautl_opts.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 14:02:24 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 14:02:26 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

const struct s_opts g_rsautl_opts[] = {
	{"-in", set_rsautl_in},
	{"-out", set_rsautl_out},
	{"-inkey", set_inkey_file},
	{"-pubin", set_rsault_pubin},
	{"-hexdump", set_hexdump},
	{"-encrypt", set_encrypt},
	{"-decrypt", set_decrypt},
	{NULL, NULL}
};

static int	rsautl_usage(char *opt)
{
	int fd;

	fd = STDERR_FILENO;
	ft_dprintf(fd, "ft_ssl: rsautl: invalid option %s\n", opt);
	ft_dprintf(fd, "Options are:\n");
	ft_dprintf(fd, "-in [file]		input file\n");
	ft_dprintf(fd, "-out[file]		output file\n");
	ft_dprintf(fd, "-inkey [file]		input key file\n");
	ft_dprintf(fd, "-pubin			input is RSA public key\n");
	ft_dprintf(fd, "-encrypt		encrypt with public key\n");
	ft_dprintf(fd, "-decrypt		decrypt with private key\n");
	ft_dprintf(fd, "-hexdump		hexdump output\n");
	return (1);
}

int			set_rsautl_in(char **argv, void *ptr, int *index)
{
	t_rsautl	*rsautl;

	rsautl = ptr;
	(*index)++;
	if (argv[*index] == NULL)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: Missing input file\n");
		return (1);
	}
	if (rsautl->in)
		free(rsautl->in);
	if ((rsautl->in = ft_strdup(argv[*index])) == NULL)
		return (1);
	return (0);
}

int			set_rsautl_out(char **argv, void *ptr, int *index)
{
	t_rsautl	*rsautl;

	rsautl = ptr;
	(*index)++;
	if (argv[*index] == NULL)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: Missing output file\n");
		return (1);
	}
	if (rsautl->out)
		free(rsautl->out);
	if ((rsautl->out = ft_strdup(argv[*index])) == NULL)
		return (1);
	return (0);
}

int			rsautl_opts(char **argv, t_rsautl *rsautl)
{
	int	i;
	int	j;

	i = 1;
	while (argv[++i])
	{
		j = -1;
		while (g_rsautl_opts[++j].opt)
		{
			if (ft_strcmp(g_rsautl_opts[j].opt, argv[i]) == 0)
			{
				if (g_rsautl_opts[j].opt_f(argv, rsautl, &i))
					return (1);
				break ;
			}
		}
		if (g_rsautl_opts[j].opt == NULL)
			return (rsautl_usage(argv[i]));
	}
	if ((rsautl->opts & (RSAUTL_ENCRYPT | RSAUTL_DECRYPT)) == 0)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: Need encrypt or decrypt option\n");
		return (1);
	}
	return (0);
}
