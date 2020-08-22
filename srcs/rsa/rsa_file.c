/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:30:04 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:30:18 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int	rsa_input_file(char *file, int fd[])
{
	if (file)
	{
		if (check_file_validity(file))
			return (1);
		if ((fd[IN] = open(file, O_RDONLY, 0644)) < 0)
		{
			ft_dprintf(STDERR_FILENO, "ft_ssl: ");
			perror(file);
			return (1);
		}
	}
	return (0);
}

int	rsa_output_file(char *file, int fd[])
{
	if (file)
	{
		if ((fd[OUT] = open(file, O_CREAT | O_TRUNC | O_WRONLY,
									0644)) < 0)
		{
			ft_dprintf(STDERR_FILENO, "ft_ssl: ");
			perror(file);
			return (1);
		}
	}
	return (0);
}
