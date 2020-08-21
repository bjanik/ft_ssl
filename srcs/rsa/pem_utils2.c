/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pem_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/21 13:20:58 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/21 13:20:59 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int			retrieve_modulus_and_public(t_rsa_data *rsa_data,
										unsigned char **ptr,
										unsigned char *data,
										uint32_t length)
{
	if (!(rsa_data->modulus = retrieve_nb_from_data(ptr, data, length)))
		return (ft_dprintf(STDERR_FILENO, "ft_ssl: asn1 decoding failed\n"));
	if (!(rsa_data->public_exp = retrieve_nb_from_data(ptr, data, length)))
		return (ft_dprintf(STDERR_FILENO, "ft_ssl: asn1 decoding failed\n"));
	return (0);
}
