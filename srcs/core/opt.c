/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 16:31:13 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 11:33:06 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int						illegal_opt(char c, char *cmd_name)
{
	ft_dprintf(STDERR_FILENO, "ft_ssl: illegal option -- %c", c);
	ft_dprintf(STDERR_FILENO, "\nusage: ft_ssl %s %s", cmd_name,
			HASH_CMD_USAGE);
	return (1);
}

static unsigned char	*hash_string(char **argv, int *ind, int i)
{
	unsigned char	*s;

	if (argv[*ind][i + 1])
		s = (unsigned char*)&argv[*ind][i + 1];
	else if (argv[*ind + 1])
		s = (unsigned char*)argv[(*ind)++ + 1];
	else
	{
		ft_dprintf(STDERR_FILENO, "%s:  option requires an argument -- s\n",
					argv[1]);
		ft_dprintf(STDERR_FILENO, "usage: ft_ssl %s %s", argv[1],
					HASH_CMD_USAGE);
		return (NULL);
	}
	return (s);
}

int						parse_opt(t_ssl_command *command, int *opts, char **av,
									int *index)
{
	int				i;
	unsigned char	*s;

	i = 0;
	while (av[*index][++i])
	{
		if (av[*index][i] == 'p')
		{
			*opts |= OPT_P;
			command->hash_func(*opts, NULL, NULL);
		}
		else if (av[*index][i] == 's')
		{
			*opts |= OPT_S;
			s = hash_string(av, index, i);
			command->hash_func(*opts, s, NULL);
			if (av[(*index) + 1] == NULL)
				break ;
		}
		else if (av[*index] && av[*index][i] != 'r' && av[*index][i] != 'q')
			return (illegal_opt(av[*index][i], av[1]));
		(av[*index][i] == 'r') ? *opts |= OPT_R : 0;
		(av[*index][i] == 'q') ? *opts |= OPT_Q : 0;
	}
	return (0);
}

int						set_options(t_ssl_command *command,
									int *opts,
									char **argv,
									int *index)
{
	int	ret;

	if ((ret = parse_opt(command, opts, argv, index)))
		return (ret);
	return (0);
}
