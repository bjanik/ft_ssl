/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_bc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 15:05:33 by bjanik            #+#    #+#             */
/*   Updated: 2018/12/05 15:05:34 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

uint64_t	des_bc_e(uint64_t plain, t_des *des)
{
	uint64_t	cipher;

	cipher = des_core(plain ^ des->init_vector, des->keys[0]);
	des->init_vector ^= cipher;
	return (cipher);
}

uint64_t	des_bc_d(uint64_t cipher, t_des *des)
{
	uint64_t	plain;

	plain = des_core(cipher, des->keys[0]) ^ des->init_vector; 
	des->init_vector ^= cipher;
	return (plain);
}
