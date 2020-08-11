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

int	rsa_input_file(t_rsa *rsa)
{
	char	*s;

	if (rsa->in)
	{
		if ((rsa->fd[IN] = open(rsa->in, O_RDONLY, 0644)) < 0)
		{
			s = (rsa->opts & RSA_PUBIN) ? "public" : "private";
			ft_dprintf(STDERR_FILENO, "ft_ssl: Error opening %s key %s\n", s,
						rsa->in);
			return (1);
		}
	}
	return (0);
}

int	rsa_output_file(t_rsa *rsa)
{
	if (rsa->out)
	{
		if ((rsa->fd[OUT] = open(rsa->out, O_CREAT | O_TRUNC | O_WRONLY,
									0644)) < 0)
			return (1);
	}
	return (0);
}
