/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pbkdf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 15:43:54 by bjanik            #+#    #+#             */
/*   Updated: 2018/12/07 15:43:56 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

/*
** Return the first 8 bytes of hash to produce key
*/

char	*pbkdf(char *password, char *salt, size_t iteration)
{
	uint64_t	dk;
	char		*hash;
	t_msg		msg;
	t_ctx		ctx;

	if (!(hash = ft_strjoin(password, salt)))
		return (NULL);
	init_msg(&msg, hash, NULL);
	while (iteration--)
	{
		hash = md5(&msg, 0);
		ft_strdel(&msg.str);
		msg.str = hash;
	}
	return (ft_strndup(hash, 8));
}
