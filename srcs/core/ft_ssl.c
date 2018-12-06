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
	{"md5", md5, HASH_CMD_OPTS, 0, NULL, NULL, NULL},
	{"sha256", sha256, HASH_CMD_OPTS, 0, NULL, NULL, NULL},
	{"sha1", sha1, HASH_CMD_OPTS, 0, NULL, NULL, NULL},
	{"des", NULL, DES_OPTS, 0, NULL, NULL, },
	{"des_bc", NULL, DES_OPTS, 0, NULL, NULL, },
	{"des_cbc", NULL, DES_OPTS, 0, NULL, NULL, },
	{"des_ecb", NULL, DES_OPTS, 0, NULL, NULL, },
	{"des_pcbc", NULL, DES_OPTS, 0, NULL, NULL, },
	// {"base64", base64, BASE64_OPTS, 0, NULL},
	{NULL, NULL, NULL, 0, NULL},
};

static void 		init_msg(void)
{
	t_msg	*msg;

	if (!(msg = (t_msg*)malloc(sizeof(t_msg))))
	{
		perror("Malloc: ");
		return (NULL);
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
			if (g_commands[i].cmd_func && !(g_command[i].msg = init_msg()))
				return (NULL);
			if (!g_commands[i].cmd_func && g_commands[i].des = init_des(cmd))
				return (NULL);
			// g_commands[i].msg->str = NULL;
			// g_commands[i].msg->msg_len = 0;
			// g_commands[i].msg->input_file = NULL;
			// g_commands[i].msg->fd = -1;
			// ft_memset(g_commands[i].msg->buf, 0, BUF_SIZE + 1);
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
