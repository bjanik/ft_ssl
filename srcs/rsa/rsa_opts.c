/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_opts.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:29:28 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:29:29 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

const struct s_opts g_rsa_opts[] = {
	{"-inform", set_inform},
	{"-outform", set_outform},
	{"-in", set_in_file},
	{"-out", set_out_file},
	{"-passin", set_password_rsa},
	{"-passout", set_password_rsa},
	{"-des", set_des_encryption},
	{"-text", set_text},
	{"-noout", set_noout},
	{"-modulus", set_modulus},
	{"-check", set_check},
	{"-pubin", set_pubin},
	{"-pubout", set_pubout},
	{NULL, NULL}
};

int		set_check(char **argv, void *ptr, int *index)
{
	t_rsa	*rsa;

	(void)argv;
	(void)index;
	rsa = ptr;
	rsa->opts |= RSA_CHECK;
	return (0);
}

int		set_text(char **argv, void *ptr, int *index)
{
	t_rsa	*rsa;

	(void)argv;
	(void)index;
	rsa = ptr;
	rsa->opts |= RSA_TEXT;
	return (0);
}

int		set_noout(char **argv, void *ptr, int *index)
{
	t_rsa	*rsa;

	(void)argv;
	(void)index;
	rsa = ptr;
	rsa->opts |= RSA_NOOUT;
	return (0);
}

int		set_modulus(char **argv, void *ptr, int *index)
{
	t_rsa	*rsa;

	(void)argv;
	(void)index;
	rsa = ptr;
	rsa->opts |= RSA_MODULUS;
	return (0);
}

int		rsa_opts(char **argv, t_rsa *rsa)
{
	int	i;
	int	j;

	i = 1;
	while (argv[++i])
	{
		j = -1;
		while (g_rsa_opts[++j].opt)
		{
			if (ft_strcmp(g_rsa_opts[j].opt, argv[i]) == 0)
			{
				if (g_rsa_opts[j].opt_f(argv, rsa, &i))
					return (1);
				break ;
			}
		}
		if (g_rsa_opts[j].opt == NULL)
		{
			ft_dprintf(STDERR_FILENO, "Unknown option: %s\n", argv[i]);
			return (1);
		}
	}
	return (0);
}
