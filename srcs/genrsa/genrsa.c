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
		bn_clears(8, &rsa->modulus, &rsa->prime1, &rsa->prime2,
					 &rsa->public_exp, &rsa->private_exp, &rsa->exponent1,
					 &rsa->exponent2, &rsa->coef);
		return (1);
	}
	return (0);
}

static int 		get_iteration_number(uint64_t bits)
{
	if (bits >= 2600)
		return (2);
	else if (bits >= 1700)
		return  (3);
	else if (bits >= 1300)
		return  (4);
	else if (bits >= 1100)
		return  (5);
	else if (bits >= 900)
		return  (6);
	else if (bits >= 800)
		return  (7);
	else if (bits >= 700)
		return  (8);
	else if (bits >= 600)
		return  (9);
	else if (bits >= 500)
		return  (10);
	else if (bits >= 400)
		return  (11);
	else if (bits >= 300)
		return  (18);
	else
		return  (27);	
}

static int 		generate_prime(t_bn *n, uint64_t bits)
{
	int iterations;

	iterations = get_iteration_number(bits);
	do {
		if (bn_set_random(n, bits / 2 + bits % 2))
			return (1);
	} while (miller_rabin(n, iterations, DISPLAY) == 1);
	return (0);
}

static void init_bns(t_bn *bns[], t_rsa_data *rsa, int numbits)
{
	bns[0] = bn_clone(rsa->prime1);
	bns[1] = bn_clone(rsa->prime2);;
	bns[2] = bn_init_size(numbits);
	bns[3] = bn_init_size(numbits);
	bns[4] = bn_clone(rsa->prime1);
}

int 		genrsa_command_run(t_rsa_data *rsa, t_genrsa *genrsa)
{
	t_bn 	*bns[5];

	if (init_rsa_data(&genrsa->rsa_data, genrsa->numbits))
		return (1);

	ft_dprintf(STDERR_FILENO, "Generating RSA private key, %d bit long modulus\n", genrsa->numbits);
	bn_set_ui(rsa->public_exp, 0x10001);
	if (generate_prime(rsa->prime1, genrsa->numbits) ||
		generate_prime(rsa->prime2, genrsa->numbits))
		return (1);
	init_bns(bns, rsa, genrsa->numbits);
	bn_mul(rsa->modulus, rsa->prime1, rsa->prime2);
	bn_sub_ui(bns[4], bns[4], 2);
	bn_mod_pow(rsa->coef, rsa->prime2, bns[4], rsa->prime1);
	ft_dprintf(STDERR_FILENO, "e is 65537 (0x10001)\n");
	bn_sub_ui(bns[0], bns[0], 1);
	bn_sub_ui(bns[1], bns[1], 1);
	bn_mul(bns[2], bns[0], bns[1]);
	bn_modinv(rsa->public_exp, bns[2], rsa->private_exp);
	bn_mod(rsa->exponent1, rsa->private_exp, bns[0]);
	bn_mod(rsa->exponent2, rsa->private_exp, bns[1]);
	bn_clears(5, &bns[0], &bns[1], &bns[2], &bns[3], &bns[4]);
	return (0);
}