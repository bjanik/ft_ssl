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
	else if (bits >= 900)
		iterations = 6;
	else if (bits >= 800)
		iterations = 7;
	else if (bits >= 700)
		iterations = 8;
	else if (bits >= 600)
		iterations = 9;
	else if (bits >= 500)
		iterations = 10;
	else if (bits >= 400)
		iterations = 11;
	else if (bits >= 300)
		iterations = 18;
	else
		iterations = 27;
	do {
		if (bn_set_random(n, bits / 2 + bits % 2))
			return (1);
	} while (miller_rabin(n, iterations, DISPLAY) == 1);
	return (0);
}

int 		genrsa_command_run(t_rsa_data *rsa, t_genrsa *genrsa)
{
	t_bn 	*p_1, *q_1, *phi, *gcd, *p_2;

	if (init_rsa_data(&genrsa->rsa_data, genrsa->numbits))
		return (1);

	ft_dprintf(STDERR_FILENO, "Generating RSA private key, %d bit long modulus\n", genrsa->numbits);
	bn_set_ui(rsa->public_exp, 0x10001);
	if (generate_prime(rsa->prime1, genrsa->numbits) ||	generate_prime(rsa->prime2, genrsa->numbits))
		return (1);
	bn_mul(rsa->modulus, rsa->prime1, rsa->prime2);
	p_1 = bn_clone(rsa->prime1);
	q_1 = bn_clone(rsa->prime2);
	phi = bn_init_size(genrsa->numbits);
	gcd = bn_init_size(genrsa->numbits);
	p_2 = bn_clone(rsa->prime1);
	if (!phi || !p_1 || !q_1 || !gcd || !p_2)
	{
		bn_clears(5, &phi, &p_1, &q_1, &gcd, &p_2);
		return (1);
	}
	bn_sub_ui(p_2, p_2, 2);
	bn_mod_pow(rsa->coef, rsa->prime2, p_2, rsa->prime1);
	ft_dprintf(STDERR_FILENO, "e is 65537 (0x10001)\n");
	bn_sub_ui(p_1, p_1, 1);
	bn_sub_ui(q_1, q_1, 1);
	bn_mul(phi, p_1, q_1);
	bn_modinv(rsa->public_exp, phi, rsa->private_exp);
	bn_mod(rsa->exponent1, rsa->private_exp, p_1);
	bn_mod(rsa->exponent2, rsa->private_exp, q_1);
	bn_clears(5, &phi, &p_1, &q_1, &gcd, &p_2);
	return (0);
}

void	genrsa_clear(t_genrsa *genrsa)
{
	bn_clear(&genrsa->rsa_data.modulus);
	bn_clear(&genrsa->rsa_data.private_exp);
	bn_clear(&genrsa->rsa_data.public_exp);
	bn_clear(&genrsa->rsa_data.prime1);
	bn_clear(&genrsa->rsa_data.prime2);
	bn_clear(&genrsa->rsa_data.exponent1);
	bn_clear(&genrsa->rsa_data.exponent2);
	bn_clear(&genrsa->rsa_data.coef);
	ft_memdel((void**)&genrsa);
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