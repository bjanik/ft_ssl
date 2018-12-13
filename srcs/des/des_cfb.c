/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_cfb.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/13 11:58:29 by bjanik            #+#    #+#             */
/*   Updated: 2018/12/13 11:58:31 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

uint64_t	des_cfb_e_d(uint64_t plain, t_des *des)
{
	uint64_t	cipher;

	cipher = des_core(des->init_vector, des->keys[0]) ^ plain;
	des->init_vector = cipher;
	return (cipher);
}
