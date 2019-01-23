/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 11:52:19 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 17:59:08 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

t_des		*init_des(char *name,
						uint64_t (*des_mode[2])(uint64_t plain, t_des *des))
{
	t_des	*des;

	if (!(des = (t_des*)malloc(sizeof(t_des))))
		ft_error_msg("ft_ssl: Malloc failed");
	des->input_file = NULL;
	des->output_file = NULL;
	des->name = name;
	des->password = NULL;
	des->salt = NULL;
	ft_memset(des->in, 0x0, BASE64_BUF_SIZE);
	ft_memset(des->keys[0], 0x0, DES_ROUNDS * sizeof(uint64_t));
	ft_memset(des->keys[1], 0x0, DES_ROUNDS * sizeof(uint64_t));
	ft_memset(des->keys[2], 0x0, DES_ROUNDS * sizeof(uint64_t));
	ft_memset(des->hex_keys, '0', MAX_KEY_LEN * 4);
	des->des_mode[0] = des_mode[0];
	des->des_mode[1] = des_mode[1];
	des->init_vector = 0;
	des->opts = 0;
	des->fd[IN] = STDIN_FILENO;
	des->fd[OUT] = STDOUT_FILENO;
	des->base64 = NULL;
	return (des);
}
