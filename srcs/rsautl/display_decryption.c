/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_decryption.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/14 15:12:48 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/14 15:12:50 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void					display_decryption(unsigned char *decrypted_msg,
											const int opts, uint32_t len,
											const int fd)
{
	if (opts & RSAUTL_HEXDUMP)
		flag_hexdump(fd, decrypted_msg, len);
	else
		write(fd, decrypted_msg, len);
}
