#include "ft_ssl.h"

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