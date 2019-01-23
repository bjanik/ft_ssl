/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:18:40 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/17 12:09:56 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

t_ssl_command		g_commands[] = {
	{"md5", md5, HASH_CMD_OPTS, 0, NULL, NULL, NULL, {NULL, NULL}},
	{"sha256", sha256, HASH_CMD_OPTS, 0, NULL, NULL, NULL, {NULL, NULL}},
	{"sha1", sha1, HASH_CMD_OPTS, 0, NULL, NULL, NULL, {NULL, NULL}},
	{"base64", 0, BASE64_OPTS, 0, NULL, NULL, NULL, {NULL, NULL}},
	{"des", 0, DES_OPTS, 0, NULL, NULL, NULL, {des_cbc_e, des_cbc_d}},
	{"des-bc", 0, DES_OPTS, 0, NULL, NULL, NULL, {des_bc_e, des_bc_d}},
	{"des-cbc", 0, DES_OPTS, 0, NULL, NULL, NULL, {des_cbc_e, des_cbc_d}},
	{"des-ecb", 0, DES_OPTS, 0, NULL, NULL, NULL, {des_ecb_e_d, des_ecb_e_d}},
	{"des-pcbc", 0, DES_OPTS, 0, NULL, NULL, NULL, {des_pcbc_e, des_pcbc_d}},
	{"des3", 0, DES_OPTS, 0, NULL, NULL, NULL, {des3_cbc_e, des3_cbc_d}},
	{"des3-bc", 0, DES_OPTS, 0, NULL, NULL, NULL, {des3_bc_e, des3_bc_d}},
	{"des3-cbc", 0, DES_OPTS, 0, NULL, NULL, NULL, {des3_cbc_e, des3_cbc_d}},
	{"des3-ecb", 0, DES_OPTS, 0, NULL, NULL, NULL, {des3_ecb_e, des3_ecb_d}},
	{"des3-pcbc", 0, DES_OPTS, 0, NULL, NULL, NULL, {des3_pcbc_e, des3_pcbc_d}},
	{NULL, 0, NULL, 0, NULL, NULL, NULL, {NULL, NULL}},
};

static t_msg		*malloc_msg(void)
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
			if (!ft_strcmp(g_commands[i].available_opts, BASE64_OPTS) &&
					!(g_commands[i].base64 = init_base64()))
				return (NULL);
			else if (g_commands[i].hash_func &&
					!(g_commands[i].msg = malloc_msg()))
				return (NULL);
			else if (!g_commands[i].hash_func && !g_commands[i].base64 &&
					!(g_commands[i].des = init_des(g_commands[i].name,
												g_commands[i].des_mode)))
				return (NULL);
			return (&g_commands[i]);
		}
	}
	return (NULL);
}

void				print_hash(unsigned char digest[],
								uint8_t digest_len,
								uint8_t up)
{
	size_t	i;

	i = 0;
	if (digest)
		while (i < digest_len)
		{
			up ? ft_printf("%02X", digest[i]) : ft_printf("%02x", digest[i]);
			i++;
		}
}
