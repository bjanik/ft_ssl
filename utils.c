// HEADER 42

#include "ft_ssl.h"

uint32_t	rotleft(uint32_t x, uint32_t n)
{
	return ((x << n) | (x >> (32 - n)));
}

uint32_t	rotright(uint32_t x, uint32_t n)
{
	return ((x >> n) | (x << (32 - n)));
}