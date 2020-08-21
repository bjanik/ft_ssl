/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_keys.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:31:12 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:31:13 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	print_encryption_header(t_des *des, const int fd)
{
	uint64_t	iv;

	if (des)
	{
		ft_dprintf(fd, "%s\n%s: DES-CBC,", PROC_TYPE, DEK_INFO);
		iv = convert_input_to_block(des->salt);
		ft_dprintf(fd, "%016llX\n\n", iv);
	}
}

void	print_rsa_key(t_rsa *rsa,
						char *data,
						const int fd)
{
	int	i;
	int	len;
	int	public;

	len = ft_strlen(data);
	i = 0;
	public = 0;
	ft_dprintf(STDERR_FILENO, "writing RSA key\n");
	if (rsa->opts & RSA_PUBIN || rsa->opts & RSA_PUBOUT)
		public = 1;
	ft_dprintf(fd, "%s\n", public ? PEM_PUBLIC_HEADER : PEM_PRIVATE_HEADER);
	if ((rsa->opts & RSA_DES) && (rsa->opts & RSA_PUBOUT) == 0)
		print_encryption_header(rsa->des, fd);
	while (len > 0)
	{
		if (len > 64)
			write(fd, data + i, 64);
		else
			write(fd, data + i, len);
		i += 64;
		len -= 64;
		write(fd, "\n", 1);
	}
	ft_dprintf(fd, "%s\n", public ? PEM_PUBLIC_FOOTER : PEM_PRIVATE_FOOTER);
}
