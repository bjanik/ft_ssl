
#include "ft_ssl.h"

uint64_t	des_ecb_e_d(uint64_t plain, t_des *des)
{
	uint64_t	cipher;

	cipher = des_core(plain, des->keys);
	return (cipher);
}