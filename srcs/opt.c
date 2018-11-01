/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 16:31:13 by bjanik            #+#    #+#             */
/*   Updated: 2018/10/31 16:31:14 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	illegal_opt(char c, char *cmd_name)
{
	write(STDERR_FILENO, "ft_ssl: illegal option -- ", 26);
	write(STDERR_FILENO, &c, 1);
 	write(STDERR_FILENO, cmd_name, ft_strlen(cmd_name));
 	exit(EXIT_FAILURE);
}

