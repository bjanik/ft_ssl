/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsautl_clear.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 14:01:56 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 14:01:58 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	rsautl_clear(t_rsautl *rsautl)
{
	ft_strdel(&rsautl->in);
	ft_strdel(&rsautl->out);
	ft_strdel(&rsautl->inkey);
	reset_des(rsautl->des);
	bn_clear(&rsautl->rsa_data.modulus);
	bn_clear(&rsautl->rsa_data.private_exp);
	bn_clear(&rsautl->rsa_data.public_exp);
	bn_clear(&rsautl->rsa_data.prime1);
	bn_clear(&rsautl->rsa_data.prime2);
	bn_clear(&rsautl->rsa_data.exponent1);
	bn_clear(&rsautl->rsa_data.exponent2);
	bn_clear(&rsautl->rsa_data.coef);
	ft_memdel((void**)&rsautl);
}
