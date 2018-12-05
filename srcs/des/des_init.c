/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 11:52:19 by bjanik            #+#    #+#             */
/*   Updated: 2018/12/05 11:52:20 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	des_init(t_des *des)
{
	des->in = NULL;
	des->out = NULL;
	ft_memset(des->input, 0x0, DES_BLOCK_SIZE);
	ft_memset(des->subkeys, 0x0, DES_ROUNDS * sizeof(uint64_t));
	des->init_vector = 0;
	des->opts = 0;	
	des->fd[IN] = STDIN_FILENO;
	des->fd[OUT] = STDOUT_FILENO;
}