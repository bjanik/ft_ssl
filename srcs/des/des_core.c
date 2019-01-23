/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/23 11:34:08 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 11:34:32 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#define LEFT 0
#define RIGHT 1

static uint32_t	f(uint32_t r_block, uint64_t key)
{
	uint64_t	exp_block;
	uint32_t	out_block;

	exp_block = expansion_permutation(r_block);
	out_block = s_box_substitutions(exp_block ^ key);
	out_block = pbox_permutation(out_block);
	return (out_block);
}

uint64_t		des_core(uint64_t block, uint64_t sub_keys[])
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
