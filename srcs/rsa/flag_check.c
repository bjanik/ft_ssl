#include "bn.h"
#include "ft_ssl.h"

static int		check_modulus(t_rsa_data rsa_data, const uint64_t size)
{
	t_bn 	*n;
	int 	ret;

	n = bn_init_size((size + 1) * 64);
	if (n == NULL)
		return (1);
	bn_mul(n, rsa_data.prime1, rsa_data.prime2);
	ret = bn_cmp(n, rsa_data.modulus);
	if (ret) 
		ft_putendl_fd("Error: modulus not equal to prime1 * prime2", STDERR_FILENO);
	bn_clear(&n);
	return (ret);
}


static int 		check_primality(t_rsa_data rsa_data)
{
	int 	ret;

	ret = 0;
	if (miller_rabin(rsa_data.prime1, 5, NO_DISPLAY) == 1)
	{
		ret = 1;
		ft_putendl_fd("Error: prime1 is not prime", STDERR_FILENO);
	}
	if (miller_rabin(rsa_data.prime1, 5, NO_DISPLAY) == 1)
	{
		ret = 1;
		ft_putendl_fd("Error: prime2 is not prime", STDERR_FILENO);
	}
	return (ret);
}


static int 		check_exponents(t_rsa_data rsa_data, const uint64_t size)
{
	t_bn *prime1_1, *prime2_1, *phi, *private_exp, *s, *gcd;

	prime1_1 = bn_clone(rsa_data.prime1);
	prime2_1 = bn_clone(rsa_data.prime2);
	phi = bn_init_size(size + 1);
	gcd = bn_init_size(size + 1);
	private_exp = bn_init_size(size + 1);
	s = bn_init_size(size + 1);
	if (!prime1_1 || !prime2_1 || !phi || !gcd || !private_exp || !s)
	{
		bn_clears(6, &prime1_1, &prime2_1, &phi, &gcd, &private_exp, &s);
		return (1);
	}
	bn_sub_ui(prime1_1, prime1_1, 1);
	bn_sub_ui(prime2_1, prime2_1, 1);
	bn_mul(phi, prime1_1, prime2_1);
	bn_gcdext(phi, rsa_data.public_exp, s, private_exp, gcd);
	display_bn(s);	
	// display_bn(private_exp);	
	if (bn_cmp_ui(gcd, 1) != 0)
	{
		ft_putendl_fd("GCD is not 1", STDERR_FILENO);
		return (1);
	}
	// display_bn(rsa_data.private_exp);
	// display_bn(private_exp);
	if (bn_cmp(private_exp, rsa_data.private_exp) != 0)
	{
		ft_putendl_fd("Values of private exponent differ", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int				flag_check(t_rsa_data rsa_data)
{
	int 		ret = 0;
	uint64_t 	size;

	size = SIZE(rsa_data.prime1) + SIZE(rsa_data.prime2);

	// if (check_primality(rsa_data))
	// 	ret = 1;
	if (check_modulus(rsa_data, size * 64))
		ret = 1;
	if (check_exponents(rsa_data, size * 64))
		ret = 1;
	if (ret == 0)
		ft_printf("RSA key ok\n");
	return (ret);
}