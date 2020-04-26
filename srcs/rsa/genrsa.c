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

static int 		generate_prime(t_bn *n, uint64_t bits)
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
		if (bn_set_random(n, bits / 2 + bits % 2))
			return (1);
	} while (miller_rabin(n, iterations, DISPLAY) == 1);
	return (0);
}


static void copy_bn_to_mpz(t_bn *n, mpz_t m)
{
	for (int64_t i = 0; i < SIZE(n); i++)
		m->_mp_d[i] = n->num[i];
	m->_mp_size = SIZE(n);
}

static void copy_mpz_to_bn(mpz_t m, t_bn *n)
{
	bn_set_zero(n);
	for (int64_t i = 0; i < ABS(m->_mp_size); i++)
		n->num[i] = m->_mp_d[i];
	n->size = ABS(m->_mp_size);
}

int 		genrsa_command_run(t_rsa_data *rsa, t_genrsa *genrsa)
{
	t_bn 	*p_1, *q_1, *phi, *gcd, *s, *p_2;
	mpz_t	mp_1, mq_1, mphi, ms, mt, mgcd, mpublic;

	if (init_rsa_data(&genrsa->rsa_data, genrsa->numbits))
		return (1);

	ft_putstr_fd("Generating RSA private key, ", STDERR_FILENO);
	ft_putnbr_fd(genrsa->numbits, STDERR_FILENO);
	ft_putendl_fd(" bit long modulus", STDERR_FILENO);
	bn_set_ui(rsa->public_exp, 0x10001);
	if (generate_prime(rsa->prime1, genrsa->numbits) ||	generate_prime(rsa->prime2, genrsa->numbits))
		return (1);
	bn_mul(rsa->modulus, rsa->prime1, rsa->prime2);
	p_1 = bn_clone(rsa->prime1);
	q_1 = bn_clone(rsa->prime2);
	phi = bn_init_size(genrsa->numbits);
	gcd = bn_init_size(genrsa->numbits);
	s = bn_init_size(genrsa->numbits);
	p_2 = bn_clone(rsa->prime1);
	if (!phi || !p_1 || !q_1 || !gcd || !s || !p_2)
	{
		bn_clears(6, &phi, &p_1, &q_1, &gcd, &s, &p_2);
		return (1);
	}
	mpz_init2(mp_1, genrsa->numbits);
	mpz_init2(mq_1, genrsa->numbits);
	mpz_init2(mgcd, genrsa->numbits);
	mpz_init2(ms, genrsa->numbits);
	mpz_init2(mt, genrsa->numbits);
	mpz_init2(mphi, genrsa->numbits);
	mpz_init2(mpublic, 64);
	mpz_set_ui(mpublic, 0x10001);
	bn_sub_ui(p_2, p_2, 2);
	bn_mod_pow(rsa->coef, rsa->prime2, p_2, rsa->prime1);
	ft_putendl_fd("e is 65537 (0x10001)", STDERR_FILENO);
	bn_sub_ui(p_1, p_1, 1);
	bn_sub_ui(q_1, q_1, 1);
	copy_bn_to_mpz(p_1, mp_1);
	copy_bn_to_mpz(q_1, mq_1);
	mpz_mul(mphi, mq_1, mp_1);
	bn_mul(phi, p_1, q_1);
	bn_modinv(rsa->public_exp, phi, rsa->private_exp);
	bn_mod(rsa->exponent1, rsa->private_exp, p_1);
	bn_mod(rsa->exponent2, rsa->private_exp, q_1);
	bn_clears(6, &phi, &p_1, &q_1, &gcd, &s, &p_2);
	return (0);
}

t_genrsa	*genrsa_init(void)
{
	t_genrsa 	*genrsa;

	if ((genrsa = malloc(sizeof(t_genrsa))) == NULL)
		return (NULL);
	genrsa->out = NULL;
	genrsa->fd[IN] = STDIN_FILENO;
	genrsa->fd[OUT] = STDOUT_FILENO;
	genrsa->numbits = 64;
	return (genrsa);
}