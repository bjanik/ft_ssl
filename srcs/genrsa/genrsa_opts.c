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
	t_genrsa *genrsa;

	genrsa = ptr;
	(*index)++;
	if ((genrsa->out = argv[*index]) == NULL)
		return (1);
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

const struct s_opts	g_genrsa_opts[] = {
	{"-o", set_genrsa_output},
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
	}
	return (finalize_genrsa_opts(genrsa));
}
