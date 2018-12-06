
#include "ft_ssl.h"

uint64_t	des_ecb_enc_dec(uint64_t plain, t_des *des)
{
	uint64_t	cipher;

	cipher = des(plain, des->keys);
	return (cipher);
}