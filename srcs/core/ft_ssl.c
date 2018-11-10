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

const t_ssl_command	g_commands[] = {
	{"md5", md5, HASH_CMD_OPTS, 0, NULL},
	{"sha256", sha256, HASH_CMD_OPTS, 0, NULL},
	{"sha1", sha1, HASH_CMD_OPTS, 0, NULL},
	{"base64", base64, BASE64_OPTS, 0, NULL},
	{NULL, NULL, NULL, 0, NULL},
};

t_ssl_command		*get_ssl_command(const char *command)
{
	int		i;

	i = -1;
	while (g_commands[++i].name)
	{
		if (!ft_strcmp(g_commands[i].name, command))
		{
			if (!(g_commands[i].msg = (t_msg*)malloc(sizeof(t_msg))))
			{
				perror("Malloc: ");
				return (NULL);
			}
			g_commands[i].msg->str = NULL;
			g_commands[i].msg->msg_len = 0;
			g_commands[i].msg->input_file = NULL;
			g_commands[i].msg->fd = -1;
			ft_memset(g_commands[i].msg->buf, 0, BUF_SIZE + 1);
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
