/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:18:40 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/17 12:09:56 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

t_ssl_command		g_commands[] = {
	{"md5", md5, {NULL, NULL}, digest_command},
	{"sha256", sha256, {NULL, NULL}, digest_command},
	{"sha1", sha1, {NULL, NULL}, digest_command},
	{"base64", NULL, {NULL, NULL}, base64_command},
	{"des", NULL, {des_cbc_e, des_cbc_d}, des_command},
	{"des-bc", NULL, {des_bc_e, des_bc_d}, des_command},
	{"des-cbc", NULL, {des_cbc_e, des_cbc_d}, des_command},
	{"des-ecb", NULL, {des_ecb_e_d, des_ecb_e_d}, des_command},
	{"des-pcbc", NULL, {des_pcbc_e, des_pcbc_d}, des_command},
	{"des3", NULL, {des3_cbc_e, des3_cbc_d}, des_command},
	{"des3-bc", NULL, {des3_bc_e, des3_bc_d}, des_command},
	{"des3-cbc", NULL, {des3_cbc_e, des3_cbc_d}, des_command},
	{"des3-ecb", NULL, {des3_ecb_e, des3_ecb_d}, des_command},
	{"des3-pcbc", NULL, {des3_pcbc_e, des3_pcbc_d}, des_command},
	{"rsa", NULL, {NULL, NULL}, rsa_command},
	{"genrsa", NULL, {NULL, NULL}, genrsa_command},
	{"rsautl", NULL, {NULL, NULL}, rsautl_command},
	{NULL, NULL, {NULL, NULL}, NULL},
};


t_ssl_command		*get_ssl_command(char **argv)
{
	int		i;

	i = -1;
	while (g_commands[++i].name)
	{
		if (!ft_strcmp(g_commands[i].name, argv[1]))
			return (&g_commands[i]);
	}
	return (NULL);
}

void				print_hash(unsigned char digest[],
								uint8_t digest_len,
								uint8_t up)
{
	size_t	i;

	i = 0;
	if (digest)
		while (i < digest_len)
		{
			up ? ft_printf("%02X", digest[i]) : ft_printf("%02x", digest[i]);
			i++;
		}
}
