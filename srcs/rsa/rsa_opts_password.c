/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_opts_password.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:26:29 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:26:30 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int	pass_env(char *var_name, char **passwd)
{
	char	*var_value;

	var_value = getenv(var_name);
	if (var_value == NULL)
	{
		ft_dprintf(2, "ft_ssl: Can't read environment variable %s\n",
					var_name);
		return (1);
	}
	if ((*passwd = ft_strdup(var_value)) == NULL)
		return (1);
	return (0);
}

int	pass_pass(char *password, char **passwd)
{
	if ((*passwd = ft_strdup(password)) == NULL)
		return (1);
	return (0);
}

int	pass_file(char *filename, char **passwd)
{
	int	fd;

	if ((fd = open(filename, O_RDONLY)) < 0)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: Can't open file %s\n", filename);
		return (1);
	}
	if (get_next_line(fd, passwd) < 0)
		return (1);
	close(fd);
	return (0);
}

int	pass_stdin(char **passwd)
{
	int	ret;

	if ((ret = get_next_line(STDIN_FILENO, passwd)) < 0)
		return (1);
	if (*passwd == NULL)
	{
		ft_dprintf(2, "ft_ssl: Failed to get password\n");
		return (1);
	}
	if (ft_strlen(*passwd) < 4)
	{
		ft_dprintf(2, "ft_ssl: Password must have at least 4 characters\n");
		return (1);
	}
	return (0);
}

int	set_password_rsa(char **argv, void *ptr, int *index)
{
	t_rsa	*rsa;
	char	**split;
	char	**str;
	int		ret;

	rsa = ptr;
	split = NULL;
	str = (!ft_strcmp(argv[(*index)], "-passout")) ? &rsa->passout :
													&rsa->passin;
	if (argv[(*index) + 1] == NULL)
		ret = (ft_dprintf(STDERR_FILENO, "ft_ssl: Missing password value\n"));
	else if (ft_strcmp(argv[++(*index)], "stdin") == 0)
		ret = pass_stdin(str);
	else if ((split = ft_strsplit(argv[*index], ':')) == NULL || !split[1])
		ret = ft_dprintf(STDERR_FILENO, "ft_ssl: Error getting password\n");
	else if (ft_strcmp(split[0], "pass") == 0)
		ret = pass_pass(split[1], str);
	else if (ft_strcmp(split[0], "env") == 0)
		ret = pass_env(split[1], str);
	else if (ft_strcmp(split[0], "file") == 0)
		ret = pass_file(split[1], str);
	else
		ret = 1;
	ft_free_string_tab(&split);
	return (ret);
}
