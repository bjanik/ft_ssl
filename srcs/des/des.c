/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/03 14:22:12 by bjanik            #+#    #+#             */
/*   Updated: 2018/12/03 14:22:13 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#define LEFT 0
#define RIGHT 1

// Converting input (type char) to block is done before calling this fonction.
// Xoring block with previous cipher block (in CBC mode) is one before calling this function.


static uint32_t f(uint32_t r_block, uint64_t key)
{
	uint64_t 	exp_block;
	uint32_t	out_block;

	exp_block = expansion_permutation(r_block);
	out_block = s_box_substitutions(exp_block ^ key);
	out_block = pbox_permutation(out_block);
	return (out_block);
}

uint64_t	des_core(uint64_t block, uint64_t sub_keys[])
{
	uint32_t	sub_block[DES_ROUNDS + 1][2];
	uint32_t	tmp;
	uint8_t		round;

	block = initial_permutation(block);
	sub_block[0][LEFT] = block >> 32;
	sub_block[0][RIGHT] = block & 0xFFFFFFFF;
	round = 0;
	while (round < DES_ROUNDS)
	{
		tmp = sub_block[round][LEFT];
		sub_block[round + 1][LEFT] = sub_block[round][RIGHT];
		sub_block[round + 1][RIGHT] = sub_block[round][LEFT] ^ 
								f(sub_block[round][RIGHT], sub_keys[round]);
		round++;
	}
	return (final_permutation(sub_block[16][LEFT], sub_block[16][RIGHT]));
}

void	cipher_to_string(uint64_t cipher, unsigned char output[])
{
	uint8_t	i;

	i = 0;
	while (i < 8)
	{
		output[i] = (unsigned char)(cipher >> (56 - i * DES_BLOCK_SIZE)) & 0xFF;
		i++;
	}
}

void	des_get_cipher(t_des *des, int offset)
{
	uint64_t		plain;
	uint64_t		cipher;

	plain = convert_input_to_block(des->input + offset);
	cipher = des->des_mode[des->opts & DES_OPT_D](plain, des);
	cipher_to_string(cipher, des->input + offset);
}

void	des_message_dec(t_des *des)
{
	int 			ret;
	int 			p_ret;
	int 			offset;

	offset = 0;
	while ((ret = read(des->fd[IN], des->input, BUF_SIZE)) > 0)
	{
		offset = 0;
		(ret % DES_BLOCK_SIZE) ? ft_error_msg("ft_ssl: bad block lenght") : 0;
		while (offset < ret)
		{
			des_get_cipher(des, offset);
			offset += DES_BLOCK_SIZE;
		}
		if (ret < BUF_SIZE)
			write(des->fd[OUT], des->input, ret - des->input[ret - 1]);
		else
			write(des->fd[OUT], des->input, offset);
		p_ret = ret;
	}
	(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
	// buflen ? write(des->fd[OUT], buffer, buflen - buffer[buflen - 1]) : 0;
}

void	des_message(t_des *des)
{
	int 			ret;
	int 			p_ret;
	int 			offset;

	p_ret = 0;
	while ((ret = read(des->fd[IN], des->input, BUF_SIZE)) > 0)
	{
		offset = 0;
		if (!(des->opts & DES_NOPAD)) 
			ft_memset(des->input + ret, 8 - ret % 8, 8 - ret % 8);
		while (offset < ret)
		{
			des_get_cipher(des, offset);
			offset += DES_BLOCK_SIZE;
		}
		write(des->fd[OUT], des->input, offset);
		p_ret = ret;
	}
	(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
	if (!(p_ret % DES_BLOCK_SIZE) && !(des->opts & DES_NOPAD))
	{
		ft_memset(des->input + ret, 8 - ret, 8 - ret);
		des_get_cipher(des, 0);
		write(des->fd[OUT], des->input, DES_BLOCK_SIZE);
	}
}