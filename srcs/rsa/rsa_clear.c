/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_clear.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:30:26 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:30:27 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void				rsa_clear(t_rsa *rsa)
{
	if (rsa)
	{
		ft_strdel(&rsa->in);
		ft_strdel(&rsa->out);
		ft_strdel(&rsa->inform);
		ft_strdel(&rsa->outform);
		ft_strdel(&rsa->passin);
		ft_strdel(&rsa->passout);
		reset_des(rsa->des);
		bn_clear(&rsa->rsa_data.modulus);
		bn_clear(&rsa->rsa_data.private_exp);
		bn_clear(&rsa->rsa_data.public_exp);
		bn_clear(&rsa->rsa_data.prime1);
		bn_clear(&rsa->rsa_data.prime2);
		bn_clear(&rsa->rsa_data.exponent1);
		bn_clear(&rsa->rsa_data.exponent2);
		bn_clear(&rsa->rsa_data.coef);
		if (rsa->in)
			close(rsa->fd[IN]);
		if (rsa->out)
			close(rsa->fd[OUT]);
	}
	ft_memdel((void**)&rsa);
}
