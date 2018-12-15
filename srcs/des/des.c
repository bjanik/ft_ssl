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

void	des_get_cipher(t_des *des, unsigned char output[])
{
	uint64_t		plain;
	uint64_t		cipher;

	plain = convert_input_to_block(des->input);
	cipher = des->des_mode[des->opts & DES_OPT_D](plain, des);
	cipher_to_string(cipher, output);
	if (!(des->opts & DES_OPT_D))
		write(des->fd[OUT], output, DES_BLOCK_SIZE);
}

void	des_message_dec(t_des *des)
{
	int 			ret;
	unsigned char 	buffer[4096 + 1];
	unsigned char 	output[DES_BLOCK_SIZE + 1];
	uint16_t		buflen;

	ft_memset(buffer, 0x0, 4096 + 1);
	buflen = 0;
	while ((ret = read(des->fd[IN], des->input, DES_BLOCK_SIZE)) > 0)
	{
		(ret < DES_BLOCK_SIZE) ? ft_error_msg("ft_ssl: bad block lenght") : 0;
		if (buflen == 4096)
		{
			write(des->fd[OUT], buffer, buflen);
			// ft_memset(buffer, 0x0, 4096);
			buflen = 0;
		}
		des_get_cipher(des, output);
		ft_memcpy(buffer + buflen, output, DES_BLOCK_SIZE);
		buflen += DES_BLOCK_SIZE;
	}
	buflen ? write(des->fd[OUT], buffer, buflen - buffer[buflen - 1]) : 0;
	(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
}

void	des_message(t_des *des)
{
	int 			ret;
	int 			prev_ret;
	unsigned char 	output[DES_BLOCK_SIZE + 1];

	ft_memset(output, 0x0, DES_BLOCK_SIZE + 1);
	prev_ret = 0;
	while ((ret = read(des->fd[IN], des->input, DES_BLOCK_SIZE)) > 0)
	{
		if (!(des->opts & DES_NOPAD))
			ft_memset(des->input + ret, 8 - ret, 8 - ret);
		des_get_cipher(des, output);
		prev_ret = ret;
	}
	if (!ret && (prev_ret == 8 || !prev_ret) && !(des->opts & DES_NOPAD))
	{
		ft_memset(des->input + ret, 8 - ret, 8 - ret);
		des_get_cipher(des, output);
	}
	(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
}