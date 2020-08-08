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

void	display_bn(t_bn *n)
{
	int64_t i;

	i = SIZE(n);
	if (SIZE(n) == 0)
		printf("0");
	else
	{	
		while (--i > -1)
		{
			if (i == SIZE(n) - 1)
				ft_printf("%llX", n->num[i]);
			else
				ft_printf("%016llX", n->num[i]);
		}
	}
	printf("\n");
}

void	display_stats(t_bn *n)
{
	ft_printf("Alloc = %llu\n",n->alloc);
	ft_printf("Size = %llu\n", n->size);
	display_bn(n);
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