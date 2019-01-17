/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_password.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 13:53:53 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/17 13:53:54 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ft_ssl.h"

char	*get_password(int encryption)
{
	char	*pwd;
	char	*check_pwd;

	if (encryption)
	{
		pwd = ft_strdup(getpass("Enter DES decryption password:"));
		check_pwd = getpass("Verifying DES decryption password:");
	}
	else
	{
		pwd = ft_strdup(getpass("Enter DES encryption password:"));
		check_pwd = getpass("Verifying DES encryption password:");
	}
	(!pwd || !check_pwd) ? ft_error_msg("ft_ssl: bad password read") : 0;
	(ft_strcmp(pwd, check_pwd)) ? ft_error_msg("ft_ssl: Verify failure") : 0;
	ft_strdel(&check_pwd);
	return (pwd);
}