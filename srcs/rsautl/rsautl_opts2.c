/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsautl_opts2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 14:02:32 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 14:02:33 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int		set_inkey_file(char **argv, void *ptr, int *index)
{
	t_rsautl	*rsautl;

	rsautl = ptr;
	(*index)++;
	if (argv[*index] == NULL)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: Missing inkey file\n");
		return (1);
	}
	if ((rsautl->inkey = ft_strdup(argv[*index])) == NULL)
		return (1);
	return (0);
}

int		set_hexdump(char **argv, void *ptr, int *index)
{
	t_rsautl	*rsautl;

	(void)argv;
	(void)index;
	rsautl = ptr;
	rsautl->opts |= RSAUTL_HEXDUMP;
	return (0);
}

int		set_encrypt(char **argv, void *ptr, int *index)
{
	t_rsautl	*rsautl;

	(void)argv;
	(void)index;
	rsautl = ptr;
	rsautl->opts |= RSAUTL_ENCRYPT;
	if (rsautl->opts & RSAUTL_DECRYPT)
		rsautl->opts &= ~RSAUTL_DECRYPT;
	return (0);
}

int		set_decrypt(char **argv, void *ptr, int *index)
{
	t_rsautl	*rsautl;

	(void)argv;
	(void)index;
	rsautl = ptr;
	rsautl->opts |= RSAUTL_DECRYPT;
	if (rsautl->opts & RSAUTL_ENCRYPT)
		rsautl->opts &= ~RSAUTL_ENCRYPT;
	return (0);
}

int		set_rsault_pubin(char **argv, void *ptr, int *index)
{
	t_rsautl	*rsautl;

	(void)argv;
	(void)index;
	rsautl = ptr;
	rsautl->opts |= RSAUTL_PUBIN;
	return (0);
}
