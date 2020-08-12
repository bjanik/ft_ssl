/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:31:31 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:31:35 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static int	init_rsa_data_decryption(t_des **des, char *line)
{
	char	**tab;
	char	**algo_iv;
	int		ret;

	ret = 0;
	if ((tab = ft_strsplit(line, ':')) == NULL)
		return (1);
	if ((algo_iv = ft_strsplit(tab[1], ',')) == NULL)
	{
		ft_free_string_tab(&tab);
		return (1);
	}
	if (ft_strcmp(algo_iv[0], " DES-CBC") == 0)
		*des = init_des("des-ecb", g_commands[6].des_mode);
	else
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: encryption algorithm not known\n");
		ret = 1;
	}
	if (ret == 0 && get_hex_from_str(algo_iv[1], &(*des)->init_vector))
		ret = 1;
	ft_free_string_tab(&tab);
	ft_free_string_tab(&algo_iv);
	return (ret);
}

static int	check_read_error(int ret, char *line)
{
	int val;

	val = 0;
	if (!line)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: Missing key footer\n");
		val = 1;
	}
	ft_strdel(&line);
	if (val == 1)
		return (1);
	if (val && ret < 0)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: Error reading key file\n");
		return (ret);
	}
	return (0);
}

static int	skip_until_header(const int fd, const char *header)
{
	int		ret;
	char	*line;
	char	*str;

	str = (!ft_strcmp(header, PEM_PRIVATE_HEADER)) ? "private" : "public";
	if ((ret = get_next_line(fd, &line)) == 0)
		return (ft_dprintf(2, "ft_ssl: Unable to load %s key\n", str));
	while (ft_strcmp(line, header))
	{
		free(line);
		ret = get_next_line(fd, &line);
		if (ret == 0)
			ft_dprintf(2, "ft_ssl: Unable to load %s key\n", str);
		else if (ret < 0)
			ft_dprintf(2, "ft_ssl: Error reading key file\n");
		if (ret < 1)
		{
			free(line);
			return (1);
		}
	}
	free(line);
	return (0);
}

static int	store_lines_in_data(char **data,
								const int fd,
								const char *footer,
								t_des **des)
{
	int		ret;
	char	*line;
	int		proc_type;

	while ((ret = get_next_line(fd, &line)) > 0)
	{
		if (ft_strcmp(line, footer) == 0)
			break ;
		else if (ft_strcmp(line, PROC_TYPE) == 0)
			proc_type = 1;
		else if (proc_type == 1 && ft_strncmp(line, DEK_INFO, 8) == 0)
			init_rsa_data_decryption(des, line);
		else if (*data == NULL)
			*data = ft_strdup(line);
		else if (*data)
			*data = ft_strjoin_free(*data, line, 1);
		ft_strdel(&line);
	}
	return (check_read_error(ret, line));
}

char		*get_data(const int fd,
						t_des **des,
						const char *header,
						const char *footer)
{
	char		*data;

	data = NULL;
	if (skip_until_header(fd, header))
		return (NULL);
	if (store_lines_in_data(&data, fd, footer, des))
		ft_strdel(&data);
	return (data);
}
