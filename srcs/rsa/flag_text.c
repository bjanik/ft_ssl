/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_text.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:25:44 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:25:46 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"
#include "ft_ssl.h"

static void	display_bn_flag_text(t_bn *n, int fd, const char *id)
{
	int64_t		pos;
	uint32_t	limb_len;
	uint32_t	printed;
	uint8_t		val;

	if (n->size == 0)
		return ;
	pos = n->size - 1;
	printed = 0;
	ft_dprintf(fd, "%s\n    ", id);
	while (pos > -1)
	{
		limb_len = get_byte_number(n->num[pos]);
		while (limb_len)
		{
			val = (n->num[pos] >> (limb_len - 1) * 8) & 0xFF;
			ft_dprintf(fd, "%02hhx", val);
			(pos != 0 || limb_len != 1) ? ft_putchar_fd(':', fd) : 0;
			printed++;
			(printed % 15 == 0) ? ft_dprintf(fd, "\n    ") : 0;
			limb_len--;
		}
		pos--;
	}
	ft_putchar_fd('\n', fd);
}

void		flag_text(t_rsa *rsa)
{
	display_bn_flag_text(rsa->rsa_data.modulus, rsa->fd[OUT], "modulus:");
	ft_dprintf(rsa->fd[OUT], "publicExponent: 65537 (0x10001)\n");
	if ((rsa->opts & RSA_PUBIN) == 0)
	{
		display_bn_flag_text(rsa->rsa_data.private_exp, rsa->fd[OUT],
								"privateExponent:");
		display_bn_flag_text(rsa->rsa_data.prime1, rsa->fd[OUT], "prime1:");
		display_bn_flag_text(rsa->rsa_data.prime2, rsa->fd[OUT], "prime2:");
		display_bn_flag_text(rsa->rsa_data.exponent1, rsa->fd[OUT],
								"exponent1:");
		display_bn_flag_text(rsa->rsa_data.exponent2, rsa->fd[OUT],
								"exponent2:");
		display_bn_flag_text(rsa->rsa_data.coef, rsa->fd[OUT], "coefficient:");
	}
}
