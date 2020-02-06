#include "gmp.h"
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

// Algo RSA using GMP

// As input, RSA key_len in bits:
# define abs(x) (x < 0) ? -1 * x : x

typedef struct 	big_num 
{
	uint64_t	*n;  // Array representing our big number
	uint64_t	size;
	uint64_t	alloc;
} 				t_bn; 


int	set_random_mpz_size(mpz_t n, size_t size)
{
	// unsigned char 	*buffer = malloc(8192);
	int 			fd;
	size_t			r, to_read;
	
	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
		return (1);
	r = size % 8;
	if (r)
		to_read = size / 8 + 1;
	else
		to_read = size / 8;
	if (read(fd, (void*)n->_mp_d, to_read) < 0)
		return (1);
	n->_mp_size = to_read / 8;
	if (to_read % 8)
		n->_mp_size++;
	if (r)
		n->_mp_d[n->_mp_size - 1] >>= 8 - size % 8;
	close(fd);
	return (0);
}



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


int main()
{
	int key_len = 4096;
	mpz_t p, p_1, q, q_1, n, phi, eps, s, t, gcd, test, test2;
	// mpz_init(p);
	printf("%d\n", p->_mp_size);
	printf("%d\n", p->_mp_alloc);
	printf("%p\n", p->_mp_d);

	gmp_randstate_t state;
	gmp_randinit_mt(state);
		
	mpz_inits(p, p_1, q, q_1, phi, n, gcd, s, t, test, test2, NULL);
	
	// We set specifically eps to 0x10001 (or 65537)
	mpz_init2(eps, 16);
	mpz_add_ui(eps, eps, 0x10001);
	// mpz_out_str(1, 10, eps);
	// display_mpz(eps);
	
	// We set test to ULLONG_MAX + 1, 65 bits
	mpz_init2(test, 1024);
	mpz_init2(test2, 1024);
	// set_random_mpz_size(test);
	mpz_add_ui(test, test, ULLONG_MAX - 10);
	mpz_add_ui(test2, test2, ULLONG_MAX);
	mpz_mul(test, test, test2);
	display_mpz(test);

	// Generation of p and q
	// mpz_urandomb(p, state, key_len / 2 - 50);
	// mpz_urandomb(q, state, key_len / 2 + 50);


	// //We make sure p and q are primes
	// if (mpz_probab_prime_p(p, 1) == 0)
	// 	mpz_nextprime(p, p);
	// if (mpz_probab_prime_p(q, 1) == 0)
	// 	mpz_nextprime(q, q);

	// // Getting n = p * q
	// mpz_mul(n, p, q);

	// // Calcutation of phi = (p-1)*(q-1).
	// mpz_sub_ui(p_1, p, 1);
	// mpz_sub_ui(q_1, q, 1);
	// mpz_mul(phi, p_1, q_1);

	// //Calculate bezout relation between phi and eps using extended Euclid algorithm
	// mpz_gcdext(gcd, s, t, phi, eps); // s * phi + eps * t = gcd ; t is the private key
	// // If gcd is not equal to 1, it means phi and eps and not coprime so we have to start all over again
	// if (gcd->_mp_size == 1 && gcd->_mp_d[0] == 1) 
	// {	
	// 	printf("phi and eps are coprime!\nPrivate key is in hexa:\n");
	// 	// display_mpz(t);
	// }

}
