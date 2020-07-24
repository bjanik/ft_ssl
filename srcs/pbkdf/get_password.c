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
	char	*pwd;
	char	*check_pwd;

	check_pwd = NULL;
	if (encryption)
	{
		pwd = ft_strdup(getpass("Enter DES decryption password:"));
		if (!pwd)
			ft_dprintf(STDERR_FILENO, "ft_ssl: bad password read\n");
	}
	else
	{
		pwd = ft_strdup(getpass("Enter DES encryption password:"));
		check_pwd = getpass("Verifying DES encryption password:");
		if (!pwd || !check_pwd)
		{
			ft_dprintf(STDERR_FILENO, "ft_ssl: bad password read");
			ft_strdel(&pwd);
		}
		else if (ft_strcmp(pwd, check_pwd))
		{
			ft_dprintf(STDERR_FILENO, "ft_ssl: Verify password failure\n");
			ft_strdel(&pwd);
		}
	}
	return (pwd);
}
