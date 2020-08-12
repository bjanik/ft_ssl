/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   genrsa_opts.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:04:35 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:04:37 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static int	set_genrsa_output(char **argv, void *ptr, int *index)
{
	t_genrsa	*genrsa;

	genrsa = ptr;
	(*index)++;
	if ((genrsa->out = argv[*index]) == NULL)
		return (1);
	return (0);
}

static int	set_genrsa_des(char **argv, void *ptr, int *index)
{
	t_genrsa	*genrsa;

	(void)argv;
	(void)index;
	genrsa = ptr;
	genrsa->opts |= GENRSA_DES;
	genrsa->des = init_des("des-cbc", g_commands[6].des_mode);
	return (0);
}

static int	finalize_genrsa_opts(t_genrsa *genrsa)
{
	if (genrsa->out)
	{
		if ((genrsa->fd[OUT] = open(genrsa->out, O_CREAT | O_WRONLY | O_TRUNC,
									0644)) < 0)
			return (1);
	}
	return (0);
}

static int	set_password_genrsa(char **argv, void *ptr, int *index)
{
	t_genrsa	*genrsa;
	char		**split;
	int			ret;

	genrsa = ptr;
	split = NULL;
	if (argv[(*index) + 1] == NULL)
		ret = (ft_dprintf(STDERR_FILENO, "ft_ssl: Missing password value\n"));
	else if (ft_strcmp(argv[++(*index)], "stdin") == 0)
		ret = pass_stdin(&genrsa->passout);
	else if ((split = ft_strsplit(argv[*index], ':')) == NULL)
		ret = 1;
	else if (ft_strcmp(split[0], "pass") == 0)
		ret = pass_pass(split[1], &genrsa->passout);
	else if (ft_strcmp(split[0], "env") == 0)
		ret = pass_env(split[1], &genrsa->passout);
	else if (ft_strcmp(split[0], "file") == 0)
		ret = pass_file(split[1], &genrsa->passout);
	else
		ret = 1;
	ft_free_string_tab(&split);
	return (ret);
}

const struct s_opts	g_genrsa_opts[] = {
	{"-o", set_genrsa_output},
	{"-des", set_genrsa_des},
	{"-passout", set_password_genrsa},
	{NULL, NULL}
};

int			genrsa_opts(char **argv, t_genrsa *genrsa)
{
	int		i;
	int		j;

	i = 1;
	while (argv[++i])
	{
		j = -1;
		if (ft_str_isdigit(argv[i]))
		{
			genrsa->numbits = ft_atoi(argv[i]);
			break ;
		}
		while (g_genrsa_opts[++j].opt)
		{
			if (ft_strcmp(g_genrsa_opts[j].opt, argv[i]) == 0)
			{
				if (g_genrsa_opts[j].opt_f(argv, genrsa, &i))
					return (1);
				break ;
			}
		}
		if (g_genrsa_opts[j].opt == NULL)
			return (genrsa_usage(argv[i]));
	}
	return (finalize_genrsa_opts(genrsa));
}
