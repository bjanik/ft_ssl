/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_bc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 14:33:01 by bjanik            #+#    #+#             */
/*   Updated: 2018/12/05 14:33:02 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

uint64_t	des_pcbc_encryption(uint64_t plain, t_des *des)
{
	uint64_t	cipher;

	cipher = des(plain ^ des->init_vector, des->keys);
	des->init_vector = cipher ^ plain;
	return (cipher);
}

uint64_t	des_pcbc_decryption(uint64_t cipher, t_des *des)
{
	uint64_t	plain;

	cipher = des(plain, des->keys) ^ des->init_vector;
	des->init_vector = plain ^ cipher;
	return (plain);
}
