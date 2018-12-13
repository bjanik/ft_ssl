
#include "ft_ssl.h"

uint64_t	des_ecb_e_d(uint64_t plain, t_des *des)
{
	return (des_core(plain, des->keys[0]));
}

uint64_t	des3_ecb(uint64_t plain, t_des *des)
{
	uint64_t	cipher;

	cipher = des_core(plain, des->keys[0]);
	cipher = des_core(cipher, des->keys[1]);
	cipher = des_core(cipher, des->keys[2]);
	return (cipher);
}