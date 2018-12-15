/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:18:40 by bjanik            #+#    #+#             */
/*   Updated: 2018/10/29 19:56:29 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

t_ssl_command		g_commands[] = {
	{"md5", md5, HASH_CMD_OPTS, 0, NULL, NULL, {NULL, NULL}},
	{"sha256", sha256, HASH_CMD_OPTS, 0, NULL, NULL, {NULL, NULL}},
	{"sha1", sha1, HASH_CMD_OPTS, 0, NULL, NULL, {NULL, NULL}},
	{"des", NULL, DES_OPTS, 0, NULL, NULL, {des_cbc_e, des_cbc_d}},
	{"des-bc", NULL, DES_OPTS, 0, NULL, NULL, {des_bc_e, des_bc_d}},
	{"des-cbc", NULL, DES_OPTS, 0, NULL, NULL, {des_cbc_e, des_cbc_d}},
	{"des-cfb", NULL, DES_OPTS, 0, NULL, NULL, {des_cfb_e_d, des_cfb_e_d}},
	{"des-ecb", NULL, DES_OPTS, 0, NULL, NULL, {des_ecb_e_d, des_ecb_e_d}},
	{"des-pcbc", NULL, DES_OPTS, 0, NULL, NULL, {des_pcbc_e, des_pcbc_d}},
	{"des3", NULL, DES_OPTS, 0, NULL, NULL, {des3_cbc_e, des3_cbc_d}},
	{"des3-bc", NULL, DES_OPTS, 0, NULL, NULL, {des3_bc_e, des3_bc_d}},
	{"des3-cbc", NULL, DES_OPTS, 0, NULL, NULL, {des3_cbc_e, des3_cbc_d}},
	{"des3-ecb", NULL, DES_OPTS, 0, NULL, NULL, {des3_ecb, des3_ecb}},
	{"des3-pcbc", NULL, DES_OPTS, 0, NULL, NULL, {des3_pcbc_e, des3_pcbc_d}},
	// {"base64", base64, BASE64_OPTS, 0, NULL},
	{NULL, NULL, NULL, 0, NULL, NULL, {NULL, NULL}},
};

t_msg 				*malloc_msg(void)
{
	t_msg	*msg;

	if (!(msg = (t_msg*)malloc(sizeof(t_msg))))
	{
		perror("Malloc: ");
		exit(EXIT_FAILURE);
	}
	msg->str = NULL;
	msg->msg_len = 0;
	msg->input_file = NULL;
	msg->fd = -1;
	ft_memset(msg->buf, 0, BUF_SIZE + 1);
	return (msg);
}

t_ssl_command		*get_ssl_command(const char *cmd)
{
	int		i;

	i = -1;
	while (g_commands[++i].name)
	{
		if (!ft_strcmp(g_commands[i].name, cmd))
		{
			if (g_commands[i].hash_func &&
					!(g_commands[i].msg = malloc_msg()))
				return (NULL);
			if (!g_commands[i].hash_func &&
					!(g_commands[i].des = init_des(g_commands[i].name,
												   g_commands[i].des_mode)))
				return (NULL);
			return (&g_commands[i]);
		}
	}
	return (NULL);
}

void				print_hash(unsigned char digest[], uint8_t digest_len)
{
	size_t	i;

	i = 0;
	if (digest)
		while (i < digest_len)
			ft_printf("%02x", digest[i++]);
}
