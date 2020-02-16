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
	z = bn_init_size(size);

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

void	test_shift_left(uint64_t val, int size, uint64_t shift)
{
	t_bn 		*x = bn_init_size(size);
	uint64_t 	blocks = size / 64;

	for (uint64_t i = 0; i < blocks; i++)
		x->num[i] = val;
	x->size = blocks;
	display_bn(*x);
	bn_shift_left(x, shift);
	display_bn(*x);
}

// void	test_display_bn_ascii(unsigned char c, int size)
// {
// 	t_bn *n = bn_init_size(size);
// 	uint64_t 	blocks = size / 64;


// 	memset((unsigned char*)n->num, c, size);
// 	display_bn_ascii(*n);
// }

void	test_sub(uint64_t val, uint64_t size)
{
	t_bn		*x, *y, *z;
	mpz_t		a, b, c;
	uint64_t 	blocks = size / 64;

	x = bn_init_size(size);
	y = bn_init_size(size);
	z = bn_init_size(size);
	mpz_init2(a, size);
	mpz_init2(b, size);
	mpz_init2(c, size);

	for (uint64_t i = 0; i < blocks; i++)
	{
		if (i % 2)
		{	
			y->num[i] = b->_mp_d[i] = val;
			z->num[i] = c->_mp_d[i] = val / 3;
		}
		else
		{
			y->num[i] = b->_mp_d[i] = val / 50;
			z->num[i] = c->_mp_d[i] = val / 555;	
		}
	}
	y->size = z->size = blocks;
	b->_mp_size = c->_mp_size = blocks;
	bn_sub(x, y, z);
	display_bn(*x);
	mpz_sub(a, b, c);
	display_mpz(a);
}

void	test_div(uint64_t val, uint64_t size)
{
	t_bn		*w, *x, *y, *z;
	mpz_t		a, b, c, d;
	uint64_t 	blocks = size / 64;

	w = bn_init_size(size);
	x = bn_init_size(size);
	y = bn_init_size(size);
	z = bn_init_size(size);
	mpz_init2(a, size);
	mpz_init2(b, size);
	mpz_init2(c, size);
	mpz_init2(d, size);

	for (uint64_t i = 0; i < blocks; i++)
	{
		y->num[i] = c->_mp_d[i] = val;
		z->num[i] = d->_mp_d[i] = val - 12;
	}
	y->size = z->size = blocks;
	bn_div(w, x, y, z);
	display_bn(*w);
	display_bn(*x);
}


void	test_sub_ui(uint64_t val, uint64_t size, uint64_t to_sub)
{
	t_bn		*y;
	mpz_t		a;

	uint64_t 	blocks = size / 64;

	y = bn_init_size(size);
	mpz_init2(a, size);

	for (uint64_t i = 0; i < blocks; i++)
		y->num[i] = a->_mp_d[i] =  val;
	y->size = a->_mp_size = blocks;

	bn_sub_ui(y, y, to_sub);
	mpz_sub_ui(a, a, to_sub);
	display_bn(*y);
	display_mpz(a);
}

// void	test_gcd(uint64_t a, uint64_t b)
// {
// 	t_bn		*x, *y, *gcd;
// 	mpz_t		u, v;

// 	y = bn_init_size(64);
// 	x = bn_init_size(64);
// 	gcd = bn_init_size(x->size);
// 	mpz_init2(u, 64);
// 	mpz_init2(v, 64);

// 	x->size = y->size = 1;
// 	u->_mp_size = v->_mp_size = 1;
// 	x->num[0] = u->_mp_d[0] = a;
// 	y->num[0] = v->_mp_d[0] = b;
// 	// bn_gcd(gcd, x, y);
// 	display_bn(*gcd);
// }

void	test_mod(uint64_t val, uint64_t size)
{
	t_bn		*r, *n, *d;
	mpz_t 		a, b, c;
	uint64_t	blocks = size / 64;

	r = bn_init_size(size);
	n = bn_init_size(size);
	d = bn_init_size(size);

	mpz_init2(a, size);
	mpz_init2(b, size);
	mpz_init2(c, size);

	for (uint64_t i = 0; i < blocks; i++)
	{
		n->num[i] = val + 1;
		d->num[i] = val;
		b->_mp_d[i] = val + 1;
		c->_mp_d[i] = val;
	}
	n->size = d->size = blocks;
	b->_mp_size = c->_mp_size = blocks;
	bn_mod(r, n, d);
	mpz_mod(a, b, c);
	display_bn(*r);
	display_mpz(a);
}

void	test_pow_mod(uint64_t ba, uint64_t expo, uint64_t modulus, uint64_t size)
{
	t_bn		*res, *base, *exp, *mod;
	mpz_t		r, b, e, m;
	uint64_t 	blocks = size / 64;

	res = bn_init_size(size);
	base = bn_init_size(size);
	exp = bn_init_size(size);
	mod = bn_init_size(size);
	mpz_init2(r, size);
	mpz_init2(b, size);
	mpz_init2(e, size);
	mpz_init2(m, size);

	base->num[0] = b->_mp_d[0] = ba;
	for (int i = 0; i < 3; i++)
		exp->num[i] = e->_mp_d[i] = expo;
	exp->size = e->_mp_size = 3;
	for (uint64_t i = 0; i < blocks; i++)
	{
		mod->num[i] = modulus * 89;
		m->_mp_d[i] = modulus * 89;
	}
	base->size = b->_mp_size= 1;
	mod->size = blocks;
	m->_mp_size = blocks;
	bn_mod_pow(res, base, exp, mod);
	mpz_powm(r, b, e, m);
	display_bn(*res);
	display_mpz(r);
}

int	test_isprime(uint64_t num)
{
	t_bn 	*n;
	mpz_t	m;

	mpz_init2(m, 64);
	n = bn_init_size(64);
	// printf("%llu\n", n->size);
	bn_add_ui(n, num);
	// printf("%llu\n", n->size);
	mpz_add_ui(m, m, num);
	int ret = pseudo_prime(n);
	// int ret = miller_rabin(n, 15);
	printf("%d\n",ret);
	printf("%d\n",mpz_probab_prime_p(m, 15));
	return ret;

}

int main()
{
	for (uint64_t i = 3; i < INT_MAX; i++)
	{
		if (i % 2)
		{
			if (test_isprime(i) == 1) // i is even so not prime
			{
				printf("ERROR on %llu\n", i);
				return 1;
			}
			printf("OK for %llu\n", i);
		}
	}

	// }
	// printf("----------------------------\n");
	// test_isprime(135);
	// printf("----------------------------\n");
	// test_isprime(7);
	// printf("----------------------------\n");
	// test_isprime(11);
	// printf("----------------------------\n");
	// test_isprime(110);
	// printf("----------------------------\n");
	// test_isprime(111);
	// printf("----------------------------\n");
	// test_isprime(114);
	// printf("----------------------------\n");
	// test_isprime(113);
	// printf("----------------------------\n");


	// test_isprime(135);
	// printf("----------------------------\n");
	// test_isprime(1000);
	// printf("----------------------------\n");
	// test_isprime(1234568);
	// printf("----------------------------\n");
	// test_isprime(10231515846848);
	// printf("----------------------------\n");
	// test_isprime(1350000000004);
	// printf("----------------------------\n");
	// // test_isprime(17);
	// printf("----------------------------\n");
	// // test_isprime(7);
	// printf("----------------------------\n");
	// // test_isprime(11);
	// printf("----------------------------\n");


	// test_pow_mod(0xFAD6498498ECB, 0x10001, 0x654654ADDD, 4096);
	// printf("----------------------------\n");
	// test_pow_mod(0xFAF, 128);
	// printf("----------------------------\n");
	// test_pow_mod(0xFAF, 128);
	// printf("----------------------------\n");
	// test_pow_mod(0xFAF, 2048);
	// printf("----------------------------\n");

	// test_mod(0x64684ADAFF, 64);
	// printf("----------------------------\n");
	// test_mod(0x684ADA, 128);
	// printf("----------------------------\n");
	// test_mod(0xFFFF0004ADA, 512);
	// printf("----------------------------\n");
	// test_mod(0xFFFF0004ADA, 4096);
	// printf("----------------------------\n");
	// test_gcd(30, 21);
	// test_sub_ui(LONG_MAX + 1, 64, ULLONG_MAX);
	// printf("----------------------------\n");
	// test_sub_ui(ULLONG_MAX, 64, ULLONG_MAX);
	// printf("----------------------------\n");
	// test_sub_ui(ULLONG_MAX, 128, 464987987);
	// printf("----------------------------\n");
	// test_sub_ui(ULLONG_MAX, 256, INT_MAX);
	// printf("----------------------------\n");
	// test_sub_ui(INT_MAX, 256, ULLONG_MAX);
	// printf("----------------------------\n");
	
	// test_div(0x88, 128);
	// test_div(21, 64);
	// test_div(0x17, 64);
	// test_div(0x10, 64);
	// test_sub(0X4648949, 512);
	// printf("----------------------------\n");

	// printf("----------------------------\n");
	// test_sub(0X46489, 512);
	// printf("----------------------------\n");
	// test_sub(0XFFFFFFF, 512);
	// printf("----------------------------\n");
	// test_sub(0XFFFF98764FFF, 1024);
	// printf("----------------------------\n");
	// test_sub(0X1, 1024);
	// printf("%llu\n", (uint64_t)(0 - 1));
	// printf("%llu\n", (uint64_t)(0 - ULLONG_MAX));
	// printf("%llu\n", (uint64_t)(0 - (ULLONG_MAX - 1)));
	// test_display_bn_ascii(65, 64);
	// test_shift_left(ULLONG_MAX - 6500004684684, 256 , 55);
	// printf("----------------------------\n");
	// test_shift_left(0x8000000000000000, 64, 32);
	// printf("----------------------------\n");
	// test_shift_left(1, 64, 1);
	// printf("----------------------------\n");
	// test_shift_left(1, 128, 64);
	// printf("----------------------------\n");
	// test_shift_left(1, 256, 128);
	// printf("----------------------------\n");
	// test_shift_left(1, 512, 256);
	// printf("----------------------------\n");
	// test_shift_left(2048, 512, 256);
	// printf("----------------------------\n");
	// test_shift_left(ULLONG_MAX, 64, 32);
	// printf("----------------------------\n");
	// test_shift_left(ULLONG_MAX, 64, 48);
	// printf("----------------------------\n");
	// test_pow(50, 64);
	// printf("----------------------------\n");
	// test_pow(12, 128);
	// printf("----------------------------\n");
	// test_pow(640, 128);
	// printf("----------------------------\n");
	// test_pow(500, 64);
	// printf("----------------------------\n");
	// test_pow(1, 128);
	// printf("----------------------------\n");
	// test_pow(8192, 128);
	// printf("----------------------------\n");


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