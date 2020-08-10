/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_password.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 13:53:53 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 11:53:33 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

char	*get_password(int encryption)
{
	char	*pwd[2];

	pwd[1] = NULL;
	if (encryption)
	{
		pwd[0] = ft_strdup(getpass("Enter DES decryption password:"));
		if (!pwd[0])
			ft_dprintf(STDERR_FILENO, "ft_ssl: bad password read\n");
	}
	else
	{
		pwd[0] = ft_strdup(getpass("Enter DES encryption password:"));
		pwd[1] = getpass("Verifying DES encryption password:");
		if (!pwd[0] || !pwd[1])
		{
			ft_dprintf(STDERR_FILENO, "ft_ssl: bad password read");
			ft_strdel(&pwd[0]);
		}
		else if (ft_strcmp(pwd[0], pwd[1]))
		{
			ft_dprintf(STDERR_FILENO, "ft_ssl: Verify password failure\n");
			ft_strdel(&pwd[0]);
		}
	}
	return (pwd[0]);
}
