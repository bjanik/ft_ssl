/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:20:05 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:20:06 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"
#include "ft_ssl.h"

static int		check_modulus(t_rsa_data rsa_data, const uint64_t size)
{
	t_bn	*n;
	int		ret;

	n = bn_init_size(size);
	if (n == NULL)
		return (1);
	bn_mul(n, rsa_data.prime1, rsa_data.prime2);
	ret = bn_cmp(n, rsa_data.modulus);
	if (ret)
		ft_dprintf(STDERR_FILENO,
					"Error: modulus not equal to prime1 * prime2\n");
	bn_clear(&n);
	return (ret);
}

static int		check_primality(t_rsa_data rsa_data)
{
	int		ret;

	ret = 0;
	if (miller_rabin(rsa_data.prime1, 5, NO_DISPLAY) == 1)
	{
		ret = 1;
		ft_dprintf(STDERR_FILENO, "Error: prime1 is not prime\n");
	}
	if (miller_rabin(rsa_data.prime1, 5, NO_DISPLAY) == 1)
	{
		ret = 1;
		ft_dprintf(STDERR_FILENO, "Error: prime2 is not prime\n");
	}
	return (ret);
}

static void		init_bns(t_bn *bns[], t_rsa_data rsa_data, const uint64_t size)
{
	bns[0] = bn_clone(rsa_data.prime1);
	bns[1] = bn_clone(rsa_data.prime2);
	bns[2] = bn_init_size(size + 1);
	bns[3] = bn_init_size(size + 1);
	bns[4] = bn_init_size(size + 1);
	bn_sub_ui(bns[0], bns[0], 1);
	bn_sub_ui(bns[1], bns[1], 1);
	bn_mul(bns[2], bns[0], bns[1]);
}

static int		check_exponents(t_rsa_data rsa_data, const uint64_t size)
{
	t_bn	*bns[5];

	init_bns(bns, rsa_data, size);
	if (bn_modinv(rsa_data.public_exp, bns[2], bns[4]) == 1)
	{
		ft_dprintf(STDERR_FILENO, "GCD is not 1\n");
		bn_clears(5, &bns[0], &bns[1], &bns[2], &bns[3], &bns[4]);
		return (1);
	}
	if (bn_cmp(bns[4], rsa_data.private_exp) != 0)
	{
		ft_dprintf(STDERR_FILENO, "Values of private exponent differ\n");
		bn_clears(5, &bns[0], &bns[1], &bns[2], &bns[3], &bns[4]);
		return (1);
	}
	bn_clears(5, &bns[0], &bns[1], &bns[2], &bns[3], &bns[4]);
	return (0);
}

int				flag_check(t_rsa_data rsa_data, int fd)
{
	int			ret;
	uint64_t	size;

	ret = 0;
	size = rsa_data.prime1->size + rsa_data.prime2->size;
	if (check_primality(rsa_data))
		ret = 1;
	if (check_modulus(rsa_data, size * 64))
		ret = 1;
	if (check_exponents(rsa_data, size * 64))
		ret = 1;
	if (ret == 0)
		ft_dprintf(fd, "RSA key ok\n");
	return (ret);
}
