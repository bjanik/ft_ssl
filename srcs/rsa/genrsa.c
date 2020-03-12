#include "bn.h"
#include "ft_ssl.h"

static int 	init_rsa_data(t_rsa_data *rsa, uint64_t numbits)
{
	rsa->modulus = bn_init_size(numbits);
	rsa->prime1 = bn_init_size(numbits / 2 + numbits % 2);
	rsa->prime2 = bn_init_size(numbits / 2 + numbits % 2);
	rsa->public_exp = bn_init_size(64);
	rsa->private_exp = bn_init_size(numbits);
	rsa->exponent1 = bn_init_size(numbits / 2 + numbits % 2);
	rsa->exponent2 = bn_init_size(numbits / 2 + numbits % 2);
	rsa->coef = bn_init_size(numbits / 2 + numbits % 2);
	if (!rsa->modulus || !rsa->prime1 || !rsa->prime2 || !rsa->public_exp ||
		!rsa->private_exp || !rsa->exponent1 || !rsa->exponent2 || !rsa->coef)
	{
		bn_clears(8, &rsa->modulus, &rsa->prime1, &rsa->prime2, &rsa->public_exp,
				  &rsa->private_exp, &rsa->exponent1, &rsa->exponent2, &rsa->coef);
		return (1);
	}
	return (0);
}

static void		generate_prime(t_bn *n, uint64_t bits)
{
	int iterations;

	if (bits >= 2600)
		iterations = 2;
	else if (bits >= 1700)
		iterations = 3;
	else if (bits >= 1300)
		iterations = 4;
	else if (bits >= 1100)
		iterations = 5;
	else
		iterations = 6;
	do {
		bn_set_random(n, bits / 2 + bits % 2);
	} while (miller_rabin(n, iterations) == 0);
}

int 		genrsa_command_run(t_rsa_data *rsa, t_genrsa *genrsa)
{
	t_bn *p_1, *q_1, *phi, *gcd, *s, *p_2; 

	if (init_rsa_data(&genrsa->rsa_data, genrsa->numbits))
		return (1);

	ft_putstr_fd("Generating RSA private key, ", STDERR_FILENO);
	ft_putnbr_fd(genrsa->numbits, STDERR_FILENO);
	ft_putendl_fd(" bit long modulus", STDERR_FILENO);
	bn_set_ui(rsa->public_exp, 0x10001);
	generate_prime(rsa->prime1, genrsa->numbits);
	generate_prime(rsa->prime2, genrsa->numbits);
	bn_mul(rsa->modulus, rsa->prime1, rsa->prime2);
	display_bn(rsa->modulus);
	p_1 = bn_clone(rsa->prime1);
	q_1 = bn_clone(rsa->prime2);
	phi = bn_init_size(genrsa->numbits);
	gcd = bn_init_size(genrsa->numbits);
	s = bn_init_size(genrsa->numbits);
	p_2 = bn_clone(rsa->prime1);
	bn_sub_ui(p_2, p_2, 2);
	bn_mod_pow(rsa->coef, rsa->prime2, p_2, rsa->prime1);
	if (!phi || !p_1 || !q_1 || !gcd || !s || !p_2)
	{
		bn_clears(6, &phi, &p_1, &q_1, &gcd, &s, &p_2);
		return (1);
	}
	ft_putendl_fd("e is 65537 (0x10001)", STDERR_FILENO);
	bn_sub_ui(p_1, p_1, 1);
	bn_sub_ui(q_1, q_1, 1);
	bn_mul(phi, p_1, q_1);
	bn_gcdext(phi, rsa->public_exp, s, rsa->private_exp, gcd);
	bn_mod(rsa->exponent1, rsa->private_exp, p_1);
	bn_mod(rsa->exponent2, rsa->private_exp, q_1);
	bn_clears(5, &phi, &p_1, &q_1, &gcd, &s);
	return (0);
}

t_genrsa	*genrsa_init(void)
{
	t_genrsa 	*genrsa;

	if ((genrsa = malloc(sizeof(t_genrsa))) == NULL)
		return (NULL);
	// genrsa->in = NULL;
	genrsa->out = NULL;
	genrsa->fd[IN] = STDIN_FILENO;
	genrsa->fd[OUT] = STDOUT_FILENO;
	genrsa->rand_file = NULL;
	genrsa->numbits = 0;
	return (genrsa);
}