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
