/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 10:33:59 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/16 19:01:26 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#define PERMUTATION_TABLE_LEN 56
#define COMPRESSION_TABLE_LEN 48

static const uint8_t	g_key_perm_table[PERMUTATION_TABLE_LEN] = {
	57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

static const uint8_t	g_key_compression_table[COMPRESSION_TABLE_LEN] = {
	14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

static const uint8_t	g_key_shift_table[DES_ROUNDS] = {
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

uint64_t				get_56bits_key(uint64_t key)
{
	uint64_t	key56;
	uint8_t		i;

	key56 = 0;
	i = 0;
	while (i < PERMUTATION_TABLE_LEN)
	{
		key56 |= ((key << (g_key_perm_table[i] - 1)) >> 63) << (55 - i);
		i++;
	}
	return (key56);
}

int						get_hex_from_str(char *str_key, uint64_t *key)
{
	unsigned char	full_key[MAX_KEY_LEN + 1];
	int				len;
	int				val;

	len = ft_strlen(str_key);
	ft_memset(full_key, '0', MAX_KEY_LEN);
	full_key[MAX_KEY_LEN] = '\0';
	(len < MAX_KEY_LEN) ? ft_memcpy(full_key, str_key, len) :
		ft_memcpy(full_key, str_key, MAX_KEY_LEN);
	len = -1;
	while (++len < MAX_KEY_LEN)
	{
		if (ft_isdigit(full_key[len]))
			val = full_key[len] - 48;
		else if (full_key[len] >= 65 && full_key[len] <= 70)
			val = full_key[len] - 55;
		else if (full_key[len] >= 97 && full_key[len] <= 102)
			val = full_key[len] - 87;
		else
		{
			ft_putendl_fd("ft_ssl: Invalid key/iv: non-hex digit",
							STDERR_FILENO);
			return (1);
		}
		*key = (*key << 4) | val;
	}
	return (0);
}

void					get_subkeys(uint32_t left_key,
									uint32_t right_key,
									uint64_t sub_keys[])
{
	uint64_t	tmp_key;
	uint64_t	sh_right_key;
	uint64_t	sh_left_key;
	int			i;
	int			j;

	i = -1;
	sh_left_key = left_key;
	sh_right_key = right_key;
	while (++i < DES_ROUNDS)
	{
		sub_keys[i] = 0;
		sh_left_key = shift_left(sh_left_key, g_key_shift_table[i]);
		sh_right_key = shift_left(sh_right_key, g_key_shift_table[i]);
		tmp_key = (sh_left_key << 28) | sh_right_key;
		j = -1;
		while (++j < COMPRESSION_TABLE_LEN)
			sub_keys[i] |= ((tmp_key >> (56 - g_key_compression_table[j]))
					& 0x1) << (47 - j);
	}
}

void	swap_keys(uint64_t keys[])
{
	uint8_t		i;
	uint64_t	tmp;

	i = 0;
	while (i < 8)
	{
		tmp = keys[i];
		keys[i] = keys[DES_ROUNDS - i - 1];
		keys[DES_ROUNDS - i - 1] = tmp;
		i++;
	}
}
