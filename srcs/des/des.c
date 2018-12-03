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

static uint64_t	des_e(uint64_t block, uint64_t sub_keys[])
{
	uint32_t	sub_block[DES_ROUNDS + 1][2];
	uint32_t	tmp;
	uint8_t		round;

	block = initial_permutation(block);
	// printf("BLOCK = %llu\n", block);
	sub_block[0][LEFT] = block >> 32;
	sub_block[0][RIGHT] = block & 0xFFFFFFFF;
	// printf("R: %u  L: %u\n", sub_block[0][RIGHT], sub_block[0][LEFT]);
	round = 0;
	while (round < DES_ROUNDS)
	{
		tmp = sub_block[round][LEFT];
		sub_block[round + 1][LEFT] = sub_block[round][RIGHT];
		sub_block[round + 1][RIGHT] = sub_block[round][LEFT] ^ 
								f(sub_block[round][RIGHT], sub_keys[round]);
		// if (round == 0)
		// 	printf("R1: %u\n", sub_block[1][RIGHT]);
		round++;
	}
	// printf("R: %u  L: %u\n", sub_block[DES_ROUNDS][RIGHT], sub_block[DES_ROUNDS][LEFT]);
	return (final_permutation(sub_block[16][LEFT], sub_block[16][RIGHT]));
}

int main(int argc, char **argv)
{
	t_des		des;
	uint64_t	key;
	uint64_t	block;

	key = 0;
	block = 0;
	get_key_from_str(argv[1], &key);
	key = get_56bits_key(key);
	get_subkeys(key >> 28, (key << 36) >> 36, des.keys);
	block = des_e(0x0123456789ABCDEF, des.keys);
	printf("%llX\n", block);
}