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


static uint32_t f(uint32_t r_block, uint64_t key)
{
	uint64_t 	exp_block;
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

void			cipher_to_string(uint64_t cipher, unsigned char output[])
{
	uint8_t	i;

	i = 0;
	while (i < 8)
	{
		output[i] = (unsigned char)(cipher >> (56 - i * DES_BLOCK_SIZE)) & 0xFF;
		i++;
	}
}

void			des_get_cipher(t_des *des, int len, unsigned char buf[])
{
	uint64_t		plain;
	uint64_t		cipher;
	int				offset;

	offset = 0;
	while (offset < len)
	{
		plain = convert_input_to_block(buf + offset);
		cipher = des->des_mode[des->opts & DES_OPT_D](plain, des);
		cipher_to_string(cipher, buf + offset);
		offset += DES_BLOCK_SIZE;
	}
	des->opts & DES_OPT_A ? base64_encode(buf, len, des->fd[OUT]) :
			 write(des->fd[OUT], buf, len);
}

static void		des_final(t_des *des, int p_ret, int len, unsigned char buf[])
{
	p_ret = 0;
	if (!(des->opts & DES_NOPAD) || !len)
	{
		ft_memset(buf + len, 8 - len % 8, 8 - len % 8);
		des_get_cipher(des, len + 8 - len % 8, buf);
	}
}

void	des_message(t_des *des)
{
	int 			ret;
	int 			p_ret;
	int 			len;
	unsigned char	buf[BASE64_BUF_SIZE + 1];

	p_ret = 0;
	len = 0;
	while ((ret = read(des->fd[IN], des->in + len, BUF_SIZE)) > 0)
	{
		!(len + ret <= BUF_SIZE) ? ft_memcpy(buf + len, des->in, ret) : 
						ft_memcpy(buf + len, des->in, BUF_SIZE - len);
		len += ret;
		if (len >= BUF_SIZE)
		{
			des_get_cipher(des, BUF_SIZE, buf);
			if (len > BUF_SIZE)
			{
				ft_memcpy(buf, des->in + ret - len + BUF_SIZE, len - BUF_SIZE);
				len -= BUF_SIZE;
			}
			else
				len = 0;
		}
		p_ret = ret;
	}
	// ft_printf("Length = %d\n", len);
	(ret < 0) ? ft_error_msg("ft_ssl: Read error") : 0;
	des_final(des, p_ret, len, buf);
	// des->opts & DES_OPT_A ? ft_putchar_fd('\n', des->fd[OUT]) : 0;
}