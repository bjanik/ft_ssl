/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_opts2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:29:47 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:29:48 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int		set_inform(char **argv, void *ptr, int *index)
{
	t_rsa	*rsa;

	rsa = ptr;
	(*index)++;
	if (argv[*index] == NULL)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: Missing inform type\n");
		return (1);
	}
	if (rsa->inform)
		free(rsa->inform);
	rsa->inform = ft_strdup(argv[*index]);
	if (rsa->inform == NULL)
		return (1);
	if (ft_strcmp("PEM", rsa->inform))
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: Incorrect input format\n");
		return (1);
	}
	return (0);
}

int		set_outform(char **argv, void *ptr, int *index)
{
	t_rsa	*rsa;

	rsa = ptr;
	(*index)++;
	if (argv[*index] == NULL)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: Missing outform type\n");
		return (1);
	}
	if (rsa->outform)
		free(rsa->outform);
	rsa->outform = ft_strdup(argv[*index]);
	if (rsa->outform == NULL)
		return (1);
	if (ft_strcmp("PEM", rsa->outform))
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: Incorrect output format\n");
		return (1);
	}
	return (0);
}

int		set_in_file(char **argv, void *ptr, int *index)
{
	t_rsa	*rsa;

	rsa = ptr;
	(*index)++;
	if (argv[*index] == NULL)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: Missing input file\n");
		return (1);
	}
	if (rsa->in)
		free(rsa->in);
	rsa->in = ft_strdup(argv[(*index)]);
	if (rsa->in == NULL)
		return (1);
	return (0);
}

int		set_out_file(char **argv, void *ptr, int *index)
{
	t_rsa	*rsa;

	rsa = ptr;
	(*index)++;
	if (argv[*index] == NULL)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: Missing output file\n");
		return (1);
	}
	if (rsa->out)
		free(rsa->out);
	rsa->out = ft_strdup(argv[(*index)]);
	if (rsa->out == NULL)
		return (1);
	return (0);
}

int		set_des_encryption(char **argv, void *ptr, int *index)
{
	t_rsa	*rsa;

	(void)argv;
	(void)index;
	rsa = ptr;
	rsa->opts |= RSA_DES;
	return (0);
}
