#include "bn.h"
#include "gmp.h"
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MIN(x, y) ((x < y) ? x : y)
#define MAX(x, y) ((x > y) ? x : y)
#define BIGGEST_BN(x, y) ((x->size >= y->size) ? x : y)
#define IS_ODD(x) ((x) & 1)


void	bn_set_zero(t_bn *n)
{
	if (n)
	{
		for (uint64_t i = 0; i < n->size; i++)
			n->num[i] = 0;
		n->size = 0;
	}
}

void	bn_concat(t_bn *n, t_bn low, t_bn high)
{
	if (n->alloc < low.size + high.size)
		bn_realloc_size(n, low.size + high.size + 1);
	bn_set_zero(n);
	while (n->size < low.size) {
		n->num[n->size] = low.num[n->size];
		n->size++;
	}
	while (n->size < low.size + high.size) {
		n->num[n->size] = high.num[n->size - low.size];
		n->size++;
	}
}

int		get_strongest_bit_pos(t_bn *n)
{
	int 		pos = 64;
	uint64_t	mask;

	mask = (uint64_t)1 << 63;

	if (n->size == 0)
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

t_bn 	*bn_init_size(uint64_t size)
{
	t_bn 	*n = NULL;
	
	n = (t_bn*)malloc(sizeof(t_bn));
	if (n == NULL)
		return (NULL);
	n->alloc = size / 64 + 1;
	n->size = 0;
	n->num = (uint64_t*)malloc(sizeof(uint64_t) * n->alloc);
	if (n->num == NULL)
		return (NULL);
	memset((void*)n->num, 0, n->alloc * 8);
	return (n);
}

t_bn	*bn_init(void)
{
	return (bn_init_size(1));
}

int 	bn_set_random(t_bn *n, uint64_t size)
{
	int to_read = 0;
	int fd = open("/dev/random", O_RDONLY);

	if (fd < 0)
		return 1;
	if (size % 8)
		to_read++;
	to_read += size / 8;
	if (read(fd, (void*)n->num, to_read) < 0)
		return 1;
	n->size = size / 64 + 1;
	close(fd);
	return 0;
}

void	display_bn(t_bn n)
{
	if (n.size == 0)
		printf("0");
	else
	{	
		for (int i = (int)n.size - 1; i > -1; i--)
		{
			if (i == (int)n.size - 1)
				printf("%llX", n.num[i]);
			else
				printf("%016llX", n.num[i]);
		}
	}
	printf("\n");
}

void	display_stats(t_bn n)
{
	printf("Alloc = %llu\n",n.alloc);
	printf("Size = %llu\n",n.size);
}

void	display_bn_ascii(t_bn n)
{
	unsigned char	*s;
	uint64_t		block;

	for (int i = (int)n.size - 1; i >= 0; i--)
	{
		block = n.num[i];
		s = (unsigned char *)block;
		printf("%s\n", s);
	}

}

int		bn_realloc(t_bn *n)
{
	uint64_t	*tmp;

	tmp = n->num;
	n->num = (uint64_t*)malloc(sizeof(uint64_t) * n->alloc * 2);
	if (n->num == NULL)
		return (1);
	memset(n->num, 0, n->alloc * 2 * sizeof(uint64_t));
	memcpy((void*)n->num, (void*)tmp, n->size * 8);
	n->alloc *= 2;
	free(tmp);
	return (0);
}

int 	bn_realloc_size(t_bn *n, uint64_t new_alloc)
{
	uint64_t 	*tmp;

	tmp = n->num;
	n->num = (uint64_t*)malloc(sizeof(uint64_t) * new_alloc);
	if (n->num == NULL)
		return (1);
	memset(n->num, 0, new_alloc * sizeof(uint64_t));
	memcpy((void*)n->num, (void*)tmp, n->size * 8);
	n->alloc = new_alloc;
	free(tmp);
	return (0);
}

int 	bn_realloc_size_zero(t_bn *n, uint64_t new_alloc)
{
	free(n->num);
	n->num = (uint64_t*)malloc(sizeof(uint64_t) * new_alloc);
	if (n->num == NULL)
		return (1);
	for (uint64_t i = 0; i < new_alloc; i++)
		n->num[i] = 0;
	n->alloc = new_alloc;
	return (0);
}

void	bn_add_ui(t_bn *n, uint64_t ui)
{
	if (n->size == 0)
	{
		n->num[n->size++] = ui;
		return ;
	}
	for (uint64_t i = 0; i < n->size; i++)
	{
		if (n->num[i] + ui < n->num[i])
		{
			n->num[i] += ui;
			ui = 1;
			if (n->size > n->alloc)
				bn_realloc(n);
		}
		else
		{
			n->num[i] += ui;
			ui = 0;
			break;
		}
	}
	if (ui == 1)
		n->num[n->size++] = ui;
}

int 	bn_cmp(t_bn *a, t_bn *b) // Return 1 if a > b, 0 if a == b, -1 if a < b
{
	if (a->size > b->size)
		return (1);
	else if (a->size < b->size)
		return (-1);
	for (int i = (int)a->size; i >= 0; i--)
	{
		if (a->num[i] > b->num[i])
			return (1);
		if (a->num[i] < b->num[i])
			return (-1);
	}
	return (0);
}

int 	bn_cmp_ui(t_bn *n, uint64_t ui)
{
	if (n->size == 1 && n->num[0] == ui)
		return (1);
	return (0);
}

t_bn 	*bn_clone(t_bn *a) // Returns a freshly allocated copy of bn a
{
	t_bn 		*n;
	uint64_t 	i;

	n = (t_bn*)malloc(sizeof(t_bn));
	if (n == NULL)
		return (NULL);
	n->num = (uint64_t*)malloc(sizeof(uint64_t) * (a->alloc + 1));
	if (n->num == NULL)
		return (NULL);
	n->size = a->size;
	n->alloc = a->alloc;
	for (i = 0 ; i < n->size; i++)
		n->num[i] = a->num[i];
	while (i < n->alloc)
		n->num[i++] = 0;
	return (n);
}

void	bn_copy(t_bn *a, t_bn *b) // Copy value of b into a. Reallocate a if necessary
{
	uint64_t i;

	if (a->alloc < b->size)
		bn_realloc_size_zero(a, b->size);
	for (i = 0 ; i < b->size; i++)
		a->num[i] = b->num[i];
	while (i < a->alloc)
		a->num[i++] = 0;
	a->size = b->size;
}

void	bn_add(t_bn *res, t_bn *op1, t_bn *op2)
{
	uint8_t 	carry = 0;
	uint64_t	max = MAX(op1->size, op2->size);
	t_bn 		*cres;
	uint64_t	i, tmp;

	if (op1->size == 0 && op2->size == 0)
	{
		bn_set_zero(res);
		return ;
	}
	if (res->alloc < op1->size + op2->size)
		bn_realloc_size(res, op1->size + op2->size + 1);
	if (op1->size == 0 || op2->size == 0)
	{
		bn_copy(res, (op1->size > op2->size) ? op1 : op2);
		return ;
	}
	cres = bn_clone(res);
	for (i = 0; i < max; i++)
	{
		tmp = op1->num[i] + op2->num[i] + carry;
		if (carry || op2->num[i])
			carry = (tmp <= op1->num[i]) ? 1 : 0;
		cres->num[i] = tmp;
		if (i >= cres->size)
			cres->size++;
	}
	if (carry)
	{
		cres->num[i] = 1;
		if (i == cres->size)
			cres->size++;
	}
	bn_copy(res, cres);
	bn_clear(&cres);
}

void 	power_of_two(t_bn *n, unsigned int pow)
{
	uint64_t 	limb;

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
	uint64_t 	limb;

	limb = pos / 64;
	n->num[limb] |=  (uint64_t)1 << (pos % 64);
	if (n->size < limb + 1)
		n->size = limb + 1;
}


void	bn_shift_right(t_bn *n, uint64_t shift)
{
	uint64_t	lost, previous_lost;
	int 		q, r, i;

	q = shift / 64;
	r = shift % 64;
	previous_lost = lost = 0;
	if (q >= (int)n->size)
		bn_set_zero(n);
	else
	{
		for (i = 0; i < (int)n->size && q > 0; i++)
		{
			if (i < q)
				n->num[i] = n->num[i + q];
			else
				n->num[i] = 0;
		}
		n->size -= q;
		for (i = (int)n->size - 1 ; i >= 0 ; i--)
		{
			previous_lost = lost;
			lost = n->num[i] << (64 - r) >> (64 - r);
			n->num[i] >>= r;
			n->num[i] |= previous_lost << (64 - r);
			if (i == (int)n->size - 1 && n->num[i] == 0)
				n->size--;
		}
	}
}

void	bn_shift_left(t_bn *n, uint64_t shift)
{
	uint64_t	lost, previous_lost;
	int 		q, r, i;

	if (n->size == 0)
		return ;
	q = shift / 64;
	r = shift % 64;
	previous_lost = lost = 0;
	if (q >= (int)n->size)
		bn_set_zero(n);
	else
	{
		for (i = (int)n->size - 1; i >= 0 && q > 0; i--)
		{
			if (i >= q)
				n->num[i] = n->num[i - q];
			else
				n->num[i] = 0;
		}
	}
	for (i = 0 ; i < (int)n->size; i++)
	{
		previous_lost = lost;
		lost = n->num[i] >> (64 - r);
		n->num[i] <<= r;
		n->num[i] |= previous_lost;
	}
	n->num[i] |= lost;
	if (n->num[i])
		n->size++;
	// i--;
	// while (i >= 0 && n->num[i] == 0)
	// {
	// 	n->size--;
	// 	i--;
	// }
}

void	print_stats(t_bn n)
{
	printf("%llu\n",n.size);
	printf("%llu\n", n.alloc);
}

void	bn_mul(t_bn *res, t_bn *m, t_bn *q)
{
	t_bn 		*a = bn_init_size(MAX(m->size, q->size) * 64 + 1000);
	t_bn		*cq = bn_clone(q);
	t_bn 		*cm = bn_clone(m);
	uint64_t 	orig_q_size = q->size;

	for (uint64_t i = 0; i < orig_q_size * 64; i++)
	{
		if (IS_ODD(cq->num[0]))
			bn_add(a, a, cm);
		bn_shift_right(cq, 1);
		cq->size = orig_q_size;
		if (IS_ODD(a->num[0]))
			cq->num[orig_q_size - 1] |= 0x8000000000000000;
		bn_shift_right(a, 1);
	}
	bn_concat(res, *cq, *a);
	bn_clear(&a);
	bn_clear(&cq);
	bn_clear(&cm);
}

void	bn_clear(t_bn **n)
{
	if (n && *n)
	{
		free((*n)->num);
		free(*n);
		*n = NULL;
	}
}

int		bn_pow(t_bn *n, t_bn *b, t_bn *exp) // Store in n value b^exp
{
	t_bn	*cb = bn_clone(b);
	t_bn	*cexp = bn_clone(exp);

	if (cb == NULL || cexp == NULL)
		return (1);
	bn_set_zero(n);
	bn_add_ui(n, 1);
	while (cexp->size > 0 || cexp->num[0] > 0)
	{
		if (IS_ODD(cexp->num[0]))
			bn_mul(n, n, cb);
		bn_mul(cb, cb, cb);
		bn_shift_right(cexp, 1);
	}
	bn_clear(&cb);
	bn_clear(&cexp);
	return (0);
}

int 	bn_mod_pow(t_bn *res, t_bn *b, t_bn *exp, t_bn *mod) // Computes b^exp % mod and stores result in res
{
	t_bn	*cb = bn_clone(b);
	t_bn	*cexp = bn_clone(exp);

	bn_set_zero(res);
	bn_add_ui(res, 1);
	bn_div(NULL, cb, cb, mod);
	while (bn_cmp_ui(cexp, 0) == 0)
	{
		if (IS_ODD(cexp->num[0]))
		{
			bn_mul(res, res, cb);
			bn_div(NULL, res, res, mod);
		}
		bn_shift_right(cexp, 1);
		bn_mul(cb, cb, cb);
		bn_div(NULL, cb, cb, mod);
	}
	bn_clear(&cb);
	bn_clear(&cexp);

}


void	bn_sub_ui(t_bn *res, t_bn *a, uint64_t ui)
{
	t_bn		*ca;
	uint64_t	j;

	if (bn_cmp_ui(a, ui) < 0)
		return ;
	if (ui == 0)
	{
		bn_copy(res, a);
		return ;
	}
	ca = bn_clone(a);
	if (ca->num[0] - ui >= ca->num[0])
	{
		j = 1;
		while (ca->num[j] == 0)
			ca->num[j++] = ULLONG_MAX;
		ca->num[j] -= 1;
	}
	bn_copy(res, ca);
	res->num[0] = ca->num[0] - ui;
	bn_clear(&ca);
}

void	bn_sub(t_bn *res, t_bn *a, t_bn *b)
{
	t_bn 		*ca;
	t_bn 		*cb;
	uint64_t	j;

	if (bn_cmp(a, b) < 0)
		return ;
	ca = bn_clone(a);
	cb = bn_clone(b);
	bn_set_zero(res);
	for (uint64_t i = 0; i < ca->size; i++)
	{
		res->num[i] = ca->num[i] - cb->num[i];
		if (cb->num[i] && res->num[i] >= ca->num[i]) // we need to substract 1 for next non zero limb and set to ULLONG_MAX its previous zero limb
		{
			j = i + 1;
			while (ca->num[j] == 0)
				ca->num[j++] = ULLONG_MAX;
			ca->num[j] -= 1;
		}
	}
	for (int i = (int)res->alloc - 1; i >= 0; i--)
	{
		if (res->num[i] != 0)
		{
			res->size = i + 1;
			break;
		}
	}
	bn_clear(&ca);
	bn_clear(&cb);
}

uint64_t 	bn_get_bit(t_bn *n, uint64_t pos)
{
	uint64_t 	limb;

	limb = pos / 64;
	// printf("LIMB = %llu\n", limb);
	return (n->num[limb] >> (pos % 64)) & 1;
}

void	bn_gcd(t_bn *gcd, t_bn *a, t_bn *b)
{
	t_bn	*rem;

	if (bn_cmp_ui(b, 0) == 0)
	{
		bn_copy(gcd, a);
		return;
	}
	rem = bn_init_size(b->size);
	bn_div(NULL, rem, a, b);
	bn_gcd(gcd, b, rem);
}

void	bn_mod(t_bn *r, t_bn *n, t_bn *d)
{
	t_bn	*cn = bn_clone(n);

	bn_set_zero(r);
	for (int i = n->size * 64 - 1; i >= 0; i--)
	{
		bn_shift_left(r, 1); // r = 2 * r
		r->num[0] ^= bn_get_bit(n, i);
		if (r->size == 0 && r->num[0] > 0)
			r->size++;
		if (bn_cmp(r, d) >= 0) // if r >= d
			bn_sub(r, r, d);
	}
	bn_clear(&cn);
}

void	bn_div(t_bn *q, t_bn *r, t_bn *n, t_bn *d)
{
	t_bn	*cn = bn_clone(n);

	bn_set_zero(r);
	bn_set_zero(q);
	for (int i = n->size * 64 - 1; i >= 0; i--)
	{
		bn_shift_left(r, 1); // rem = 2 * rem
		r->num[0] ^= bn_get_bit(n, i);
		if (r->size == 0 && r->num[0] > 0)
			r->size++;
		if (bn_cmp(r, d) >= 0) // if rem >= den
		{
			bn_sub(r, r, d);
			if (q)
			{
				bn_set_bit(q, i);
				if (i / 64)
					q->size = i / 64;
				else
					q->size = 1;
			}
		}
	}
	bn_clear(&cn);
}