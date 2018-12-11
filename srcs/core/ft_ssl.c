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
	{"md5", md5, HASH_CMD_OPTS, 0, NULL, NULL},
	{"sha256", sha256, HASH_CMD_OPTS, 0, NULL, NULL},
	{"sha1", sha1, HASH_CMD_OPTS, 0, NULL, NULL},
	{"des", NULL, DES_OPTS, 0, NULL, NULL},
	{"des-bc", NULL, DES_OPTS, 0, NULL, NULL},
	{"des-cbc", NULL, DES_OPTS, 0, NULL, NULL},
	{"des-ecb", NULL, DES_OPTS, 0, NULL, NULL},
	{"des-pcbc", NULL, DES_OPTS, 0, NULL, NULL},
	// {"base64", base64, BASE64_OPTS, 0, NULL},
	{NULL, NULL, NULL, 0, NULL, NULL},
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
					!(g_commands[i].des = init_des(cmd)))
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
