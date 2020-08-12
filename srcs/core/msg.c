/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 15:03:05 by bjanik            #+#    #+#             */
/*   Updated: 2018/10/29 19:56:07 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int		init_msg(t_msg *msg, unsigned char *message, char *input_file)
{
	ft_memset(msg, 0x0, sizeof(t_msg));
	if (message)
	{
		msg->str = message;
		msg->msg_len = ft_strlen((char*)message);
		msg->fd = -1;
		msg->input_file = NULL;
	}
	else if (input_file)
	{
		msg->str = NULL;
		msg->msg_len = 0;
		if ((msg->fd = open(input_file, O_RDONLY)) < 0)
		{
			write(STDERR_FILENO, "ft_ssl: ", 8);
			perror(input_file);
			return (1);
		}
		msg->input_file = input_file;
	}
	else
		msg->fd = STDIN_FILENO;
	return (0);
}

void	reset_msg(t_msg *msg)
{
	msg->str = NULL;
	msg->msg_len = 0;
	msg->input_file = NULL;
	msg->fd = -1;
}
