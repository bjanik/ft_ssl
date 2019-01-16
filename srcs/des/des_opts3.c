/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_opts3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/16 14:46:01 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/16 18:52:37 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int		set_encrypt_mode(char **argv, t_des *des, int *index)
{
	(void)argv;
	(void)index;
	des->opts &= ~DES_OPT_D;
	return (0);
}

int		set_password(char **argv, t_des *des, int *index)
{
	if (!argv[++(*index)])
	{
		ft_putendl_fd("ft_ssl: Password must be defined", STDERR_FILENO);
		return (1);
	}
	if (!(des->password = ft_strdup(argv[*index])))
		ft_error_msg("Malloc failed");
	return (0);
}

int		set_cap_p(char **argv, t_des *des, int *index)
{
	(void)argv;
	(void)index;
	des->opts |= DES_OPT_CAP_P;
	return (0);
}

int		set_base64(char **argv, t_des *des, int *index)
{
	(void)argv;
	(void)index;
	des->opts |= DES_OPT_A;
	if (!(des->base64 = ft_memalloc(sizeof(t_base64))))
		ft_error_msg("Malloc failed");
	return (0);
}
