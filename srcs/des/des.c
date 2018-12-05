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

static uint64_t	des(uint64_t block, uint64_t sub_keys[])
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
	while (i < sizeof(uint64_t))
	{
		output[i] = (unsigned char)(cipher >> (56 - i * DES_BLOCK_SIZE));
		i++;
	}
}

void	des_message(t_des *des)
{
	int 			ret;
	uint64_t		plain;
	unsigned char 	output[DES_BLOCK_SIZE + 1];

	ft_memset(output, 0x0, DES_BLOCK_SIZE);
	while ((ret = read(des->fd[IN], des->input, DES_BLOCK_SIZE)) > 0)
	{
		if (ret < DES_BLOCK_SIZE)
			ft_memset(des->input + ret, 0x0, DES_BLOCK_SIZE - ret);
		plain = convert_input_to_block(input);
		// If CBC : block ^= des->init_vector;
		cipher = des(plain, des->keys);
		// IF CBC: des->init_vector = cipher;
		cipher_to_string(cipher, output);
		write(des->fd[OUT], output, DES_BLOCK_SIZE);
	}
	if (ret < 0)
	{
		perror("ft_ssl: Read: ");
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char **argv)
{
	t_des			des;
	uint64_t		key;
	uint64_t		block;
	unsigned char 	input[DES_BLOCK_SIZE + 1];
	// unsigned char 	output[DES_BLOCK_SIZE + 1];

	key = 0;
	block = 0;
	get_data_from_str(argv[1], &key);
	key = get_56bits_key(key);
	get_subkeys(key >> 28, (key << 36) >> 36, des.keys);
	// des.fd[IN] = open(argv[2], O_RDONLY, 0644);
	des.fd[IN] = STDIN_FILENO;
	des.fd[OUT] = STDOUT_FILENO;

	des_message(input, des.fd, des.keys);
	// block = des_e(0x0123456789ABCDEF, des.keys);
	// // block = des_e(0x85E813540F0AB405, des.keys);
	// printf("%016llX\n", block);
	// block_to_string(block, output);
	// printf("%s\n", output);
	return (0);
}