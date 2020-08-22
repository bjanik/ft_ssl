/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/22 14:52:04 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/22 14:52:05 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int	check_file_validity(const char *file)
{
	struct stat		stats;
	int				ret;

	ret = 0;
	stat(file, &stats);
	if (stats.st_mode & S_IFDIR)
		ret = ft_dprintf(STDERR_FILENO, "ft_ssl: %s: Is a directory\n", file);
	return (ret);
}
