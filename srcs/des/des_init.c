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

void	set_des_mode(t_des	*des, char *des_mode)
{
	if (!ft_strcmp(des_mode, "des_ecb"))
	{
		des->des_mode[ENCRYPT] = des_ecb_encryption;
		des->des_mode[DECRYPT] = des_ecb_decryption;
	}
	else if (!ft_strcmp(des_mode, "des_cbc"))
	{
		des->des_mode[ENCRYPT] = des_cbc_encryption;
		des->des_mode[DECRYPT] = des_cbc_decryption;
	}
	else if (!ft_strcmp(des_mode, "des_bc"))
	{
		des->des_mode[ENCRYPT] = des_bc_encryption;
		des->des_mode[DECRYPT] = des_bc_decryption;
	}
	else if (!ft_strcmp(des_mode, "des_pcbc"))
	{
		des->des_mode[ENCRYPT] = des_pcbc_encryption;
		des->des_mode[DECRYPT] = des_pcbc_decryption;
	}
}

t_des	*init_des(char *des_mode)
{
	t_des	*des;

	if (!(des = (t_des*)malloc(sizeof(t_des))))
	{
		perror("Malloc:");
		return (NULL);
	}
	des->in = NULL;
	des->out = NULL;
	ft_memset(des->input, 0x0, DES_BLOCK_SIZE);
	ft_memset(des->subkeys, 0x0, DES_ROUNDS * sizeof(uint64_t));
	des->init_vector = 0;
	des->opts = 0;	
	des->fd[IN] = STDIN_FILENO;
	des->fd[OUT] = STDOUT_FILENO;
	return (des);
}