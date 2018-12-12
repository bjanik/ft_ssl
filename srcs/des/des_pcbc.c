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

uint64_t	des_pcbc_e(uint64_t plain, t_des *des)
{
	uint64_t	cipher;

	cipher = des_core(plain ^ des->init_vector, des->keys);
	des->init_vector = cipher ^ plain;
	return (cipher);
}

uint64_t	des_pcbc_d(uint64_t cipher, t_des *des)
{
	uint64_t	plain;

	plain = des_core(cipher, des->keys) ^ des->init_vector;
	des->init_vector = plain ^ cipher;
	return (plain);
}
