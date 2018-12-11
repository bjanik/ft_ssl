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

#define ENCRYPT 0
#define DECRYPT 1

static void	set_des_mode(t_des *des, const char *des_mode)
{
	if (!ft_strcmp(des_mode, "des-ecb"))
	{
		des->des_mode[ENCRYPT] = des_ecb_enc_dec;
		des->des_mode[DECRYPT] = des->des_mode[ENCRYPT];
	}
	else if (!ft_strcmp(des_mode, "des-cbc") || !ft_strcmp(des_mode, "des"))
	{
		des->des_mode[ENCRYPT] = des_cbc_encryption;
		des->des_mode[DECRYPT] = des_cbc_decryption;
	}
	else if (!ft_strcmp(des_mode, "des-bc"))
	{
		des->des_mode[ENCRYPT] = des_bc_encryption;
		des->des_mode[DECRYPT] = des_bc_decryption;
	}
	else if (!ft_strcmp(des_mode, "des-pcbc"))
	{
		des->des_mode[ENCRYPT] = des_pcbc_encryption;
		des->des_mode[DECRYPT] = des_pcbc_decryption;
	}
}

t_des		*init_des(const char *des_mode)
{
	t_des	*des;

	if (!(des = (t_des*)malloc(sizeof(t_des))))
	{
		perror("Malloc:");
		exit(EXIT_FAILURE);
	}
	des->in = NULL;
	des->out = NULL;
	ft_memset(des->input, 0x0, DES_BLOCK_SIZE);
	ft_memset(des->keys, 0x0, DES_ROUNDS * sizeof(uint64_t));
	set_des_mode(des, des_mode);
	des->init_vector = 0;
	des->opts = 0;	
	des->fd[IN] = STDIN_FILENO;
	des->fd[OUT] = STDOUT_FILENO;
	return (des);
}