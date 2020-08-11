/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passphrase.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:31:19 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:31:22 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

char		*get_pem_passphrase(const char *in, int decrypt)
{
	char		*pwd[2];
	char		*prompt;

	prompt = (decrypt) ? ft_strnjoin("Enter pass phrase for ", 2, in, ":") :
							ft_strdup("Enter PEM pass phrase:");
	if (prompt == NULL)
		return (NULL);
	pwd[0] = ft_strdup(getpass(prompt));
	free(prompt);
	if (decrypt || pwd[0] == NULL)
		return (pwd[0]);
	if (ft_strlen(pwd[0]) < 4)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: password needs at least 4 bytes\n");
		ft_strdel(&pwd[0]);
		ft_strdel(&prompt);
		return (NULL);
	}
	pwd[1] = (!decrypt) ? getpass("Verifying - Enter PEM pass phrase:") : 0;
	if (pwd[0] == NULL || pwd[1] == NULL)
		ft_dprintf(STDERR_FILENO, "ft_ssl: bad PEM password read\n");
	else if (ft_strcmp(pwd[0], pwd[1]) != 0)
		ft_dprintf(STDERR_FILENO, "ft_ssl: Verify password failure\n");
	return (pwd[0]);
}

static int	set_passwd(t_des *des,
							const int decryption,
							const char *in,
							const char *pass)
{
	if (pass == NULL)
		des->password = get_pem_passphrase(in, decryption);
	else
		des->password = ft_strdup(pass);
	if (des->password == NULL)
		return (1);
	return (0);
}

int			key_from_passphrase(t_des *des,
									const int decryption,
									const char *in,
									const char *pass)
{
	unsigned char	*hash;
	unsigned char	keys[4][8];

	if (set_passwd(des, decryption, in, pass))
		return (1);
	if (des->init_vector == 0)
	{
		if (get_salt(des))
			return (1);
	}
	else
	{
		des->salt = (unsigned char *)ft_malloc(8 * sizeof(unsigned char));
		cipher_to_string(des->init_vector, des->salt);
	}
	if (!(hash = pbkdf(des->password, des->salt, SINGLE_DES)))
		return (1);
	set_key(des, hash, keys, SINGLE_DES);
	ft_memdel((void**)&hash);
	des->init_vector = convert_input_to_block(des->salt);
	return (0);
}
