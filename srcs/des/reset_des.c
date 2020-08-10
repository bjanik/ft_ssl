/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_des.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 11:52:36 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 11:52:40 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void		reset_des(t_des *des)
{
	if (des)
	{
		if (des->input_file)
			close(des->fd[IN]);
		if (des->output_file)
			close(des->fd[OUT]);
		ft_strdel(&des->password);
		ft_strdel((char**)&des->salt);
		ft_memdel((void**)&des->base64);
		ft_memdel((void**)&des);
	}
}
