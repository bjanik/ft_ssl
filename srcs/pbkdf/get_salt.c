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

unsigned char 	*get_salt(t_des *des)
{
	int 			fd;
	unsigned char	*salt;

	if (!(salt = ft_memalloc(sizeof(char) * 9)))
		ft_error_msg("ft_ssl: malloc failed");
	// ft_memset(salt, 0x0, 9);
	if ((fd = open("/dev/random", O_RDONLY, 0644)) < 0)
		ft_error_msg("ft_ssl: /dev/random opening failed");
	if (read(fd, salt, 8) < 0)
	{
		close(fd);	
	 	ft_error_msg("ft_ssl: read failed");
	}
	close(fd);
	return (salt);
}
