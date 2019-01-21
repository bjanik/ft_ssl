/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_salt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 13:52:59 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/17 13:53:00 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int 	get_salt(t_des *des)
{
	int 			fd;

	des->salt = (unsigned char*)malloc(sizeof(unsigned char*) * 8);
	if ((fd = open("/dev/random", O_RDONLY, 0644)) < 0)
		ft_error_msg("ft_ssl: /dev/random opening failed");
	if (read(fd, des->salt, 8) < 0)
	{
		close(fd);	
	 	ft_putendl_fd("ft_ssl: read failed", STDERR_FILENO);
	 	return (1);
	}
	// print_hash(des->salt, 8, 1);
	close(fd);
	return (0);
}
