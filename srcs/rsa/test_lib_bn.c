#include "bn.h"
#include "gmp.h"
#include <stdio.h>

void	display_mpz(mpz_t n)
{
	for (int i = abs(n->_mp_size) - 1; i > -1; i--)
	{
		if (i == (int)n->_mp_size - 1)
			printf("%lX", n->_mp_d[i]);
		else
			printf("%016lX", n->_mp_d[i]);
	}
	printf("\n");
}

void	test_mul(uint64_t val, uint64_t size)
{
	t_bn	*x, *y, *z;
	mpz_t	a, b, c;

	x = bn_init_size(size);
	y = bn_init_size(size);
	z = bn_init_size(size);

	mpz_init2(a, size);
	mpz_init2(b, size);
	mpz_init2(c, size);

	for (uint64_t i = 0; i < size / 64; i++)
	{
		if (i % 2)
		{
			x->num[i] = y->num[i] = val / i;
			a->_mp_d[i] = b->_mp_d[i] = val / i;
		}
		else
		{
			x->num[i] = y->num[i] = val + i*i;
			a->_mp_d[i] = b->_mp_d[i] = val + i*i;
		}
	}
	x->size = y->size =  size / 64;
	a->_mp_size = b->_mp_size = size / 64;
	// display_bn(*x);
	// display_bn(*y);
	mpz_mul(c, a, b);
	display_mpz(c);
	bn_mul(z, y, x);
	display_bn(*z);
	bn_clear(&x);
	bn_clear(&y);
	bn_clear(&z);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(c);
}

void	test_add(uint64_t val, uint64_t size)
{
	t_bn	*x, *y, *z;
	mpz_t	a, b, c;
	uint64_t blocks = size / 64;

	x = bn_init_size(size);
	y = bn_init_size(size);
	z = bn_init_size(size);

	mpz_init2(a, size);
	mpz_init2(b, size);
	mpz_init2(c, size);

	for (uint64_t i = 0; i < size / 64; i++)
	{

		x->num[i] = a->_mp_d[i] = val;
		if (i != blocks - 1)
			y->num[i] = b->_mp_d[i] = val;
	}
	x->size = a->_mp_size = blocks;
	y->size = b->_mp_size = blocks - 1;
	bn_add(z, x, y);
	mpz_add(c, a, b);
	display_bn(*z);
	display_mpz(c);
	bn_clear(&x);
	bn_clear(&y);
	bn_clear(&z);
	mpz_clear(a);
	mpz_clear(b);
	// mpz_clear(c);
}

void	test_pow(uint64_t val, uint64_t size)
{
	t_bn		*x, *y, *z;
	mpz_t		a, b, c;
	uint64_t 	blocks = size / 64;

	x = bn_init_size(size);
	y = bn_init_size(64);
	z = bn_init_size(65536);

	mpz_init2(a, size);
	mpz_init2(b, size);
	mpz_init2(c, size);

	for (uint64_t i = 0; i < size / 64; i++)
	{
		x->num[i] = a->_mp_d[i] = val;
		b->_mp_d[i] = val;
	}
	x->size = a->_mp_size = blocks;
	b->_mp_size = blocks;
	y->size = 1;
	y->num[0] = val;
	mpz_pow_ui(a, b, val);
	printf("MPZ : \n");
	display_mpz(a);
	bn_pow(z, x, y);
	printf("BN : \n");
	display_bn(*z);
}

int main()
{
	printf("----------------------------\n");
	test_pow(50, 64);
	printf("----------------------------\n");
	test_pow(12, 128);
	printf("----------------------------\n");
	test_pow(640, 128);
	printf("----------------------------\n");
	test_pow(500, 64);
	printf("----------------------------\n");
	test_pow(1, 128);
	printf("----------------------------\n");


	// test_add(10, 64);
	// printf("----------------------------\n");
	// test_add(654984684, 128);
	// printf("----------------------------\n");
	// test_add(1649654, 256);
	// printf("----------------------------\n");
	// test_add(7984654984, 4096);
	// printf("----------------------------\n");
	// test_add(798460054984, 8192);
	// printf("----------------------------\n");
	// test_add(798, 512);
	// printf("----------------------------\n");
	// test_add(7984984, 512);
	// printf("----------------------------\n");
	// test_add(9984654984, 512);
	// printf("----------------------------\n");
	// test_add(0xFF54654, 1024);
	// printf("----------------------------\n");


	// test_mul(ULLONG_MAX / INT_MAX, 2048);
	// printf("----------------------------\n");
	// test_mul(150, 64);
	// printf("----------------------------\n");
	// test_mul(654984894, 64);
	// printf("----------------------------\n");
	// test_mul(987456321120, 64);
	// printf("----------------------------\n");
	// test_mul(INT_MAX * 8, 64);
	// printf("----------------------------\n");
	// test_mul(1000, 64);
	// printf("----------------------------\n");
	// test_mul(8978789456, 512);
	// printf("----------------------------\n");
	// test_mul(1789000000000000700, 64);
	// printf("----------------------------\n");
	// test_mul(1, 128);
	// printf("----------------------------\n");
	// test_mul(79864900000, 256);
	// printf("----------------------------\n");
	// test_mul(798640007, 512);
	// printf("----------------------------\n");
	// test_mul(79860000, 384);
	// printf("----------------------------\n");
	// test_mul(165459860000, 1024);
	// printf("----------------------------\n");
	// test_mul(ULLONG_MAX, 1024);
	// printf("----------------------------\n");
	// test_mul(ULLONG_MAX - INT_MAX * 200, 8192);
	return 0;
}