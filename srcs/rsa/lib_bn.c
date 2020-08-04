#include "bn.h"
#include "ft_ssl.h"
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MIN(x, y) ((x < y) ? x : y)
#define MAX(x, y) ((x > y) ? x : y)
#define IS_ODD(x) ((x) & 1)
#define IS_EVEN(x) (((x) & 1) == 0)

uint32_t bn_get_bit_number(t_bn *n)
{
	unsigned int 	bits = 64;
	uint64_t		mask = 0x8000000000000000;
	int64_t			size;

	if (SIZE(n) == 0)
		return (0);
	while ((n->num[SIZE(n) - 1] & mask) == 0)
	{
		bits--;
		mask >>= 1;
	}
	size = SIZE(n);
	while (--size)
		bits += 64;
	return (bits);

}

uint32_t get_byte_number(uint64_t limb)
{
	if (limb <= 0xFF)
		return (1);
	if (limb <= 0xFFFF)
		return (2);
	if (limb <= 0xFFFFFF)
		return (3);
	if (limb <= 0xFFFFFFFF)
		return (4);
	if (limb <= 0xFFFFFFFFFF)
		return (5);
	if (limb <= 0xFFFFFFFFFFFF)
		return (6);
	if (limb <= 0xFFFFFFFFFFFFFF)
		return (7);
	return (8);
}

uint32_t bn_len(t_bn *n)
{
    unsigned int    bytes;

    if (SIZE(n) == 0)
    	return (0);
    bytes = (SIZE(n) - 1) * 8;
    return (bytes + get_byte_number(n->num[SIZE(n) - 1]));
}


uint64_t 	bn_get_bit(t_bn *n, uint64_t pos)
{
	uint64_t 	limb;

	limb = pos / 64;
	return (n->num[limb] >> (pos % 64)) & 1;
}


int		get_strongest_bit_pos(t_bn *n)
{
	int 		pos = 64;
	uint64_t	mask;

	mask = (uint64_t)1 << 63;

	if (SIZE(n) == 0)
		pos = 0;
	else
	{
		while ((n->num[n->size - 1] & mask) == 0)
		{
			mask >>= 1;
			pos--;
		}
	}
	return (pos);
}

void	display_bn(t_bn *n)
{
	if (SIZE(n) == 0)
		printf("0");
	else
	{	
		for (int64_t i = SIZE(n) - 1; i > -1; i--)
		{
			if (i == SIZE(n) - 1)
				printf("%llX", n->num[i]);
			else
				printf("%016llX", n->num[i]);
		}
	}
	printf("\n");
}

void	display_stats(t_bn *n)
{
	printf("Alloc = %llu\n",n->alloc);
	printf("Size = %llu\n", n->size);
	display_bn(n);
}

void	display_bn_ascii(t_bn n)
{
	unsigned char	*s;
	uint64_t		block;

	for (int64_t i = n.size - 1; i >= 0; i--)
	{
		block = n.num[i];
		s = (unsigned char *)block;
		printf("%s\n", s);
	}

}

int		bn_copy(t_bn *a, t_bn *b) // Copy value of b into a. Reallocate a if necessary
{
	int64_t i;

	if (a->alloc < SIZE(b))
	{
		if (bn_realloc_size_zero(a, SIZE(b)))
			return (1);
	}
	for (i = 0 ; i < SIZE(b); i++)
		a->num[i] = b->num[i];
	while (i < a->alloc)
		a->num[i++] = 0;
	a->size = b->size;
	return (0);
}

void 	power_of_two(t_bn *n, unsigned int pow)
{
	int64_t 	limb;

	limb = pow / 64;
	while (limb >= n->alloc)
		bn_realloc(n);
	bn_set_zero(n);
	n->num[limb] = 1;
	n->num[limb] <<= pow % 64;
	n->size = limb + 1;
}

void	bn_set_bit(t_bn *n, uint64_t pos)
{
	int64_t 	limb;

	limb = pos / 64;
	n->num[limb] |=  (uint64_t)1 << (pos % 64);
	if (n->size < limb + 1)
		n->size = limb + 1;
}

void	print_stats(t_bn n)
{
	printf("%llu\n",n.size);
	printf("%llu\n", n.alloc);
}


int		bn_pow(t_bn *res, t_bn *b, t_bn *exp)
{
	t_bn	*cb = bn_clone(b);
	t_bn	*cexp = bn_clone(exp);
	t_bn	*cres = bn_clone(res);

	if (!cb || !cexp || !cres)
	{
		bn_clears(3, &cb, &cexp, &cres);
		return (1);
	}
	bn_set_zero(cres);
	bn_add_ui(cres, 1);
	while (SIZE(cexp) > 0 || cexp->num[0] > 0)
	{
		if (IS_ODD(cexp->num[0]))
			bn_mul(cres, cres, cb);
		bn_mul(cb, cb, cb);
		bn_shift_right(cexp, 1);
	}
	bn_copy(res, cres);
	bn_clear(&cb);
	bn_clear(&cexp);
	bn_clear(&cres);
	return (0);
}