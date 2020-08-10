/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_opts3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:29:54 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:29:56 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int	set_pubin(char **argv, void *ptr, int *index)
{
	t_rsa	*rsa;

	(void)argv;
	(void)index;
	rsa = ptr;
	rsa->opts |= RSA_PUBIN;
	return (0);
}

int	set_pubout(char **argv, void *ptr, int *index)
{
	t_rsa	*rsa;

	(void)argv;
	(void)index;
	rsa = ptr;
	rsa->opts |= RSA_PUBOUT;
	return (0);
}
