/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/24 12:55:06 by bjanik            #+#    #+#             */
/*   Updated: 2018/11/24 12:55:17 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ssl.h"

void			init_processing(t_base64 *base)
{
	base->fd[IN] = STDIN_FILENO;
	base->fd[OUT] = STDOUT_FILENO;
	if (base->input_file)
		if ((base->fd[IN] = open(base->input_file, O_RDONLY, 0644)) < 0)
		{
			ft_putstr_fd("ft_ssl: ", STDERR_FILENO);
			perror(base->input_file);
		}
	if (base->output_file)
		if ((base->fd[OUT] = open(base->output_file, O_WRONLY |
													O_CREAT |
													O_TRUNC, 0644)) < 0)
		{
			ft_putstr_fd("ft_ssl: ", STDERR_FILENO);
			perror(base->input_file);
		}
}