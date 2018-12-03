/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 14:09:57 by bjanik            #+#    #+#             */
/*   Updated: 2018/11/28 14:09:58 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ft_ssl.h"

#define INIT_PERM_TABLE_LEN 64
#define FINAL_PERM_TABLE_LEN 64
#define EXP_PERM_TABLE_LEN 48
#define PBOX_PERM_TABLE_LEN 32

static const uint8_t	g_init_perm_table[INIT_PERM_TABLE_LEN] = {
	58, 50, 42, 34, 26, 18, 10 , 2, 60, 52, 44, 36, 28, 20 , 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

static const uint8_t	g_final_perm_table[FINAL_PERM_TABLE_LEN] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25
};

static const uint8_t	g_exp_perm_table[EXP_PERM_TABLE_LEN] = {
	32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1  
};

static const uint8_t 	g_pbox_perm_table[PBOX_PERM_TABLE_LEN] = {
	16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
	2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};

uint64_t	convert_input_to_block(unsigned char input[])
{
	uint64_t	block;
	uint64_t	tmp;
	uint8_t		i;

	block = 0;
	i = 0;
	while (i < 8)
	{
		tmp = input[i];
		block |= tmp << (56 - 8 * i);
		i++; 
	}
	return (block);
}

uint64_t	initial_permutation(uint64_t block)
{
	uint8_t		i;
	uint64_t	new_block;

	i = 0;
	new_block = 0;
	while (i < INIT_PERM_TABLE_LEN)
	{
		new_block |= ((block << (64 - g_init_perm_table[i])) >> 63) << i;
		i++;
	}
	return (new_block);
}

uint64_t	expansion_permutation(uint32_t r_block)
{
	uint64_t	exp_block;
	uint64_t	tmp;
	uint8_t		i;

	i = 0;
	exp_block = 0;
	tmp = r_block;
	while (i < EXP_PERM_TABLE_LEN)
	{
		exp_block |= ((tmp << (64 - g_exp_perm_table[i])) >> 63) << i;
		i++;
	}
	return (exp_block);
}

uint32_t	pbox_permutation(uint32_t block)
{
	uint32_t new_block;
	uint8_t i;

	i = 0;
	new_block = 0;
	while (i < PBOX_PERM_TABLE_LEN)
	{
		new_block |= ((block >> (32 - g_pbox_perm_table[i])) & 0x1) << (31 - i);
		i++;
	}
	return (new_block);
}

uint64_t	final_permutation(uint32_t left, uint32_t right)
{
	uint64_t f_block;
	uint8_t	 i;

	f_block = right;
	f_block = (f_block << 32) | left;
	i = 0;
	printf("FINAL BLOCK BEFORE FP: %llu\n", f_block);
	while (i < FINAL_PERM_TABLE_LEN)
	{
		printf("%llu", (f_block >> (64 - g_final_perm_table[i])) & 0x1);
		f_block |= ((f_block >> (64 - g_final_perm_table[i])) & 0x1) << (63 - i);
		i++;
	}
	printf("\n");
	return (f_block);
}


// int main(int argc, char **argv)
// {
// 	unsigned char	input[8];
// 	uint64_t		block, new_block, exp_block, final_block;
// 	uint32_t		right_block, left_block;

// 	input[0] = 0x01;
// 	input[1] = 0x23;
// 	input[2] = 0x45;
// 	input[3] = 0x67;
// 	input[4] = 0x89;
// 	input[5] = 0xAB;
// 	input[6] = 0xCD;
// 	input[7] = 0xEF;
// 	block = convert_input_to_block(input);
// 	new_block = initial_permutation(block);
// 	left_block = new_block >> 32;
// 	right_block = (new_block << 32) >> 32;
// 	exp_block = expansion_permutation(right_block);
// 	final_block = final_permutation(1128411700, 172808597);
// 	printf("final_block = %llu\n", final_block);
// 	return (0);
// }