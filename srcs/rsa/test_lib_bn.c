#include "bn.h"
#include "gmp.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

# define ITERATION 512

int 	cmp_bn_mpz(t_bn *n, mpz_t m) // n and m have same size
{
	for (int64_t i = 0; i < n->size; i++)
	{
		if (n->num[i] != m->_mp_d[i])
		{
			printf("ERROR\n");
			return 1;
		}
	}
	return 0;
}

void	set_randomly_mpz_bn(t_bn *n, mpz_t m, int64_t size)
{
	unsigned char buffer[4096];
	int to_read = 0;

	if (size % 8)
		to_read++;
	to_read += size / 8;
	int fd = open("/dev/random", O_RDONLY);
	read(fd, buffer, to_read);
	memcpy((uint8_t*)n->num, buffer, to_read);
	memcpy((uint8_t*)m->_mp_d, buffer, to_read);
	n->size = size / 64;
	if (size % 64)
		n->size++;
	m->_mp_size = n->size;
	close(fd);
}

void	display_mpz(mpz_t n)
{
	for (int64_t i = n->_mp_size - 1; i > -1; i--)
	{
		if (i == n->_mp_size - 1)
			printf("%lX", n->_mp_d[i]);
		else
			printf("%016lX", n->_mp_d[i]);
	}
	printf("\n");
}

void	test_mul(uint64_t size)
{
	t_bn	*x, *y, *z;
	mpz_t	a, b, c;

	x = bn_init_size(size);
	y = bn_init_size(size);
	z = bn_init_size(size);

	mpz_init2(a, size);
	mpz_init2(b, size);
	mpz_init2(c, size);

	set_randomly_mpz_bn(y, a, size);
	set_randomly_mpz_bn(x, b, size);
	mpz_mul(c, a, b);
	bn_mul(z, y, x);
	cmp_bn_mpz(z, c);
	bn_clears(3, &x, &y, &z);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(c);
}

void	test_add(int64_t size)
{
	t_bn	*x, *y, *z;
	mpz_t	a, b, c;

	x = bn_init_size(size);
	y = bn_init_size(size);
	z = bn_init_size(size);

	mpz_init2(a, size);
	mpz_init2(b, size);
	mpz_init2(c, size);

	set_randomly_mpz_bn(y, a, size);
	set_randomly_mpz_bn(x, b, size);
	bn_add(z, x, y);
	mpz_add(c, a, b);
	cmp_bn_mpz(z, c);
	bn_clears(3, &x, &y, &z);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(c);
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

void	test_shift_left(int size, uint64_t shift)
{
	t_bn 		*x = bn_init_size(size);
	mpz_t		a;

	mpz_init2(a, size);
	set_randomly_mpz_bn(x, a, size);

	display_bn(*x);
	bn_shift_left(x, shift);
	display_bn(*x);
	bn_clear(&x);
}

void	test_shift_right(int size, uint64_t shift)
{
	t_bn 		*x = bn_init_size(size);
	mpz_t		a;

	mpz_init2(a, size);
	set_randomly_mpz_bn(x, a, size);


	display_bn(*x);
	bn_shift_right(x, shift);
	display_bn(*x);
	bn_clear(&x);
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

void	test_sub(int64_t size)
{
	t_bn		*x, *a, *b;
	mpz_t 		n, p, q;

	x = bn_init_size(size);
	a = bn_init_size(size);
	b = bn_init_size(size);

	mpz_init2(n, size);
	mpz_init2(p, size);
	mpz_init2(q, size);	

	set_randomly_mpz_bn(x, n, size);
	set_randomly_mpz_bn(a, p, size);
	set_randomly_mpz_bn(b, q, size);

	bn_sub(x, a, b);
	mpz_sub(n, p, q);
	bn_clears(3, &x, &a, &b);
	mpz_clear(n);
	mpz_clear(p);
	mpz_clear(q);
}

void	test_mod(int64_t size)
{
	t_bn		*r, *n, *d;
	mpz_t 		a, b, c;

	r = bn_init_size(size);
	n = bn_init_size(size);
	d = bn_init_size(size);

	mpz_init2(a, size);
	mpz_init2(b, size);
	mpz_init2(c, size);
	// set_randomly_mpz_bn(r, a, size);
	set_randomly_mpz_bn(n, b, size);
	set_randomly_mpz_bn(d, c, size);

	bn_mod(r, n, d);
	mpz_mod(a, b, c);
	cmp_bn_mpz(r, a);
	bn_clears(3, &r, &n, &d);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(c);
}

void	test_pow_mod(uint64_t size)
{
	t_bn		*res, *base, *exp, *mod;
	mpz_t		r, b, e, m;

	res = bn_init_size(size);
	base = bn_init_size(size);
	exp = bn_init_size(size);
	mod = bn_init_size(size);
	mpz_init2(r, size);
	mpz_init2(b, size);
	mpz_init2(e, size);
	mpz_init2(m, size);

	set_randomly_mpz_bn(base, b, 64);
	set_randomly_mpz_bn(exp, e, 128);
	set_randomly_mpz_bn(mod, m, size);

	bn_mod_pow(res, base, exp, mod);
	mpz_powm(r, b, e, m);
	cmp_bn_mpz(res, r);
	bn_clears(4, &res, &base, &exp, &mod);
	mpz_clear(r);
	mpz_clear(b);
	mpz_clear(e);
	mpz_clear(m);
}

int	test_isprime(uint64_t size)
{
	t_bn 			*n = NULL;
	mpz_t			m;
	int 			ret = 0;
	int 			ret2 = 0;

	mpz_init2(m, size);
	n = bn_init_size(size);
	set_randomly_mpz_bn(n, m, size);
	ret = miller_rabin(n, 5);
	ret2 = mpz_probab_prime_p(m, 10);
	bn_clear(&n);
	mpz_clear(m);
	printf("%d %d\n",ret, ret2);
	return ret;
}

int test_gcd(int64_t size)
{
	t_bn *gcd, *x, *y;
	mpz_t g, b, c;

	gcd = bn_init_size(size);
	x = bn_init_size(size);
	y = bn_init_size(size);

	mpz_init2(g, size);
	mpz_init2(b, size);
	mpz_init2(c, size);

	set_randomly_mpz_bn(x, b, size);
	set_randomly_mpz_bn(y, c, size);

	bn_gcd(gcd, x, y);
	mpz_gcd(g, b, c);
	cmp_bn_mpz(gcd, g);
	bn_clears(3, &gcd, &x, &y);
	mpz_clear(g);
	mpz_clear(b);
	mpz_clear(c);
	return 0;

}

int 	test_gcd_u(int64_t val_a, int64_t val_b)
{
	// Calculate extented euclid 
	int64_t	gcd, s, t, a, b;
	mpz_t gcdm, sm, tm, am, bm;

	mpz_init2(gcdm, 64);
	mpz_init2(sm, 64);
	mpz_init2(tm, 64);
	mpz_init2(am, 64);
	mpz_init2(bm, 64);

	mpz_add_ui(am, am, (uint64_t)val_a);
	mpz_add_ui(bm, bm, (uint64_t)val_b);
	a = val_a;
	b = val_b;
	ext_euclid(&a, &b, &s, &t, &gcd);
	mpz_gcdext(gcdm, sm, tm, am, bm);

	printf("a = %lld am = %lu\n", a, am->_mp_d[0]);
	printf("b = %lld bm = %lu\n", b, bm->_mp_d[0]);
	printf("gcd = %lld gcdm = %lu\n", gcd, gcdm->_mp_d[0]);
	printf("t = %lld tm = %lu\n", t, tm->_mp_d[0]);
	printf("s = %lld sm = %lu\n", s, sm->_mp_d[0]);
}

int test_gcdext(int64_t size)
{
	t_bn 	*x, *y, *s, *t, *gcd;
	mpz_t 	a, b, s1, t1, g;

	printf("START GCDEXT TEST\n");

	gcd = bn_init_size(size);
	x = bn_init_size(size);
	y = bn_init_size(size);
	s = bn_init_size(size);
	t = bn_init_size(size);

	mpz_init2(a, size);
	mpz_init2(b, size);
	mpz_init2(s1, size);
	mpz_init2(t1, size);
	mpz_init2(g, size);

	set_randomly_mpz_bn(x, a, size);
	set_randomly_mpz_bn(y, b, size);
	printf("BEFORE GCDEXT CALL\n");
	bn_gcdext(x, y, s, t, gcd);
	printf("AFTER GCDEXT CALL\n");
	mpz_gcdext(g, s1, t1, a, b);
	// for (int64_t i = 0; i < gcd->size; i++)
	// {
	// 	if (gcd->num[i] != g->_mp_d[i])
	// 	{
	// 		printf("ERROR\n");
	// 		return 1;
	// 	}
	// }
	// for (int64_t i = 0; i < s->size; i++)
	// {
	// 	if (s->num[i] != s1->_mp_d[i])
	// 	{
	// 		printf("ERROR\n");
	// 		return 1;
	// 	}
	// }
	// for (int64_t i = 0; i < t->size; i++)
	// {
	// 	if (t->num[i] != t1->_mp_d[i])
	// 	{
	// 		printf("ERROR\n");
	// 		return 1;
	// 	}
	// }
	bn_clears(5, &gcd, &x, &y, &s, &t);
	mpz_clear(g);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(s1);
	mpz_clear(t1);
	printf("END GCDEXT TEST\n");
	return 0;
}

int main()
{
	test_gcd_u(12, 42);
	// for (int i = 0; i < ITERATION; i++)
	// {
	// 	test_isprime(64);
	// 	test_isprime(128);
	// 	test_isprime(256);
	// 	test_isprime(384);
	// 	test_isprime(512);
	// 	test_isprime(1024);
	// 	test_isprime(768);
	// 	test_isprime(2048);
	// }
	// printf("PRIME PASSED\n");
	// for (int i = 0 ; i < ITERATION; i++)
	// {
	// 	test_sub(64);
	// 	test_sub(256);
	// 	test_sub(512);
	// 	test_sub(128);
	// 	test_sub(1024);
	// 	test_sub(2048);
	// }
	// printf("SUB PASSED\n");
	// for (int i = 0 ; i < ITERATION; i++)
	// {
	// 	test_add(64);
	// 	test_add(256);
	// 	test_add(512);
	// 	test_add(64);
	// 	test_add(128);
	// 	test_add(1024);
	// 	test_add(2048);
	// }
	// printf("ADD PASSED\n");
	// for (int i = 0 ; i < ITERATION; i++)
	// {
	// 	test_mod(64);
	// 	test_mod(256);
	// 	test_mod(512);
	// 	test_mod(64);
	// 	test_mod(128);
	// 	test_mul(1024);
	// 	test_mul(2048);
	// }
	// printf("MOD PASSED\n");
	// for (int i = 0 ; i < ITERATION; i++)
	// {
	// 	test_mul(64);
	// 	test_mul(256);
	// 	test_mul(512);
	// 	test_mul(64);
	// 	test_mul(128);
	// 	test_mul(1024);
	// 	test_mul(2048);
	// }
	// printf("MUL PASSED\n");	
	// for (int i = 0 ; i < ITERATION; i++)
	// {
	// 	test_pow_mod(1024);
	// 	test_pow_mod(512);
	// 	test_pow_mod(64);
	// 	test_pow_mod(128);
	// 	test_pow_mod(384);
	// 	test_pow_mod(256);
	// }
	// printf("POW MOD PASSED\n");
	// while (1);
	return 0;
}