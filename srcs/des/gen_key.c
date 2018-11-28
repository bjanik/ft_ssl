/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 10:33:59 by bjanik            #+#    #+#             */
/*   Updated: 2018/11/27 10:34:00 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ft_ssl.h"

#define MAX_KEY_LEN 16
#define PERMUTATION_TABLE_LEN 56
#define COMPRESSION_TABLE_LEN 48


static const uint8_t	key_perm_table[PERMUTATION_TABLE_LEN] = {
	57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

static const uint8_t	key_compression_table[COMPRESSION_TABLE_LEN] = {
	14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

static const uint8_t	key_shift_table[DES_ROUNDS] = {
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};


void	get_key_from_str(char *str_key, uint64_t *key)
{
	unsigned char		full_key[MAX_KEY_LEN + 1];
	int					len;
	int					val;
 
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
			ft_putendl_fd("ft_ssl: Invalid key: non-hex digit", STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
		*key = (*key << 4) | val;
	}
}

uint64_t	get_56bits_key(uint64_t key)
{
	uint64_t	skey;
	uint8_t		i;

	skey = 0;
	i = 0;
	while (i < PERMUTATION_TABLE_LEN)
	{
		skey |= ((key << (key_perm_table[i] - 1)) >> 63) << i; 
		i++;
	}
	return (skey);
}

inline static void	split_key(uint64_t skey, uint32_t *right_key, uint32_t *left_key)
{
	*left_key = skey >> 28;
	*right_key = (skey << 28) >> 28;
}

void	get_subkeys(uint32_t right_key, uint32_t left_key)
{
	uint64_t	sub_keys[DES_ROUNDS];
	uint64_t	tmp_key;
	int			i;
	int			j;

	i = -1;
	while (++i < DES_ROUNDS)
	{
		sub_keys[i] = 0;
		tmp_key = (rotleft(left_key, key_shift_table[i]) << 28) | 
					rotleft(right_key, key_shift_table[i]);
		j = -1;
		while (++j < COMPRESSION_TABLE_LEN)
			sub_keys[i] |= 
				((tmp_key << (key_compression_table[i] - 1)) >> 63) << i;
	}
	i = -1;
	while (++i < DES_ROUNDS)
		printf("subkey %d: %llu\n", i, sub_keys[i]);

}


int main(int argc, char **argv)
{
	uint64_t 	ull;
	uint32_t	left_key;
	uint32_t	right_key;
	uint64_t	skey;

	if (argc > 1)
	{
		get_key_from_str(argv[1], &ull);
		printf("%llu\n", ull);
	}
	skey = get_56bits_key(ull);
	split_key(skey, &right_key, &left_key);
	get_subkeys(right_key, left_key);
	return (0);
}