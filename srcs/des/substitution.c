/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 18:06:48 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/16 18:48:26 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#define NB_SUB_BLOCKS 8
#define SBOX_LEN 64

const uint8_t g_s_box1[SBOX_LEN] = {
	14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
	0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
	4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
	15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
};

const uint8_t g_s_box2[SBOX_LEN] = {
	15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
	3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
	0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
	13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
};

const uint8_t g_s_box3[SBOX_LEN] = {
	10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
	13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
	13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
	1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
};

const uint8_t g_s_box4[SBOX_LEN] = {
	7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
	13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
	10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
	3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
};

const uint8_t g_s_box5[SBOX_LEN] = {
	2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
	14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
	4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
	11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
};

const uint8_t g_s_box6[SBOX_LEN] = {
	12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
	10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
	9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
	4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
};

const uint8_t g_s_box7[SBOX_LEN] = {
	4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
	13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
	1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
	6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
};

const uint8_t g_s_box8[SBOX_LEN] = {
	13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
	1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
	7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
	2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
};

static void		fill_sub_blocks(uint8_t sub_blocks[], uint64_t x_block)
{
	uint8_t	i;

	i = 0;
	while (i < NB_SUB_BLOCKS)
	{
		sub_blocks[i] = (x_block >> ((NB_SUB_BLOCKS - i - 1) * 6)) & 0x3F;
		i++;
	}
}

static uint8_t	s_box(uint8_t sub_block, const uint8_t s_box[])
{
	uint8_t		row;
	uint8_t		col;

	row = (sub_block & 0x1) | ((sub_block >> 5) << 1);
	col = (sub_block >> 1) & 0xF;
	return (s_box[col + row * 16]);
}

uint32_t		s_box_substitutions(uint64_t x_block)
{
	uint8_t		sub_blocks[NB_SUB_BLOCKS];
	uint8_t		sbox_output[NB_SUB_BLOCKS];
	uint32_t	out_block;

	out_block = 0;
	fill_sub_blocks(sub_blocks, x_block);
	sbox_output[0] = s_box(sub_blocks[0], g_s_box1);
	sbox_output[1] = s_box(sub_blocks[1], g_s_box2);
	sbox_output[2] = s_box(sub_blocks[2], g_s_box3);
	sbox_output[3] = s_box(sub_blocks[3], g_s_box4);
	sbox_output[4] = s_box(sub_blocks[4], g_s_box5);
	sbox_output[5] = s_box(sub_blocks[5], g_s_box6);
	sbox_output[6] = s_box(sub_blocks[6], g_s_box7);
	sbox_output[7] = s_box(sub_blocks[7], g_s_box8);
	out_block |= sbox_output[0] << 28;
	out_block |= sbox_output[1] << 24;
	out_block |= sbox_output[2] << 20;
	out_block |= sbox_output[3] << 16;
	out_block |= sbox_output[4] << 12;
	out_block |= sbox_output[5] << 8;
	out_block |= sbox_output[6] << 4;
	out_block |= sbox_output[7];
	return (out_block);
}
