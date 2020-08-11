/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   genrsa_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:04:42 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:04:44 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

t_genrsa	*genrsa_init(void)
{
	t_genrsa	*genrsa;

	if ((genrsa = ft_malloc(sizeof(t_genrsa))) == NULL)
		return (NULL);
	genrsa->out = NULL;
	genrsa->fd[IN] = STDIN_FILENO;
	genrsa->fd[OUT] = STDOUT_FILENO;
	genrsa->des = NULL;
	genrsa->numbits = 64;
	ft_memset((void*)&genrsa->rsa_data, 0x0, sizeof(t_rsa_data));
	return (genrsa);
}
