#include "bn.h"
#include "gmp.h"
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MIN(x, y) ((x < y) ? x : y)
#define MAX(x, y) ((x > y) ? x : y)
#define IS_ODD(x) ((x) & 1)
#define IS_EVEN(x) (((x) & 1) == 0)



void	bn_set_zero(t_bn *n)
{
	if (n)
	{
		for (int64_t i = 0; i < SIZE(n); i++)
			n->num[i] = 0;
		n->size = 0;
	}
}

void	bn_set_ui(t_bn *n, uint64_t ui)
{
	bn_set_zero(n);
	if (ui > 0)
	{
		n->size = 1;
		n->num[0] = ui;
	}
}

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

uint32_t bn_get_byte_number_last_limb(uint64_t limb)
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

uint32_t bn_get_byte_number(t_bn *n)
{
    unsigned int    bytes;

    if (SIZE(n) == 0)
    	return (0);
    bytes = (SIZE(n) - 1) * 8;
    return (bytes + bn_get_byte_number_last_limb(n->num[SIZE(n) - 1]));
}


uint64_t 	bn_get_bit(t_bn *n, uint64_t pos)
{
	uint64_t 	limb;

	limb = pos / 64;
	return (n->num[limb] >> (pos % 64)) & 1;
}

void	bn_concat(t_bn *n, t_bn *low, t_bn *high)
{
	if (n->alloc < SIZE(low) + SIZE(high))
		bn_realloc_size(n, SIZE(low) + SIZE(high) + 1);
	bn_set_zero(n);
	while (SIZE(n) < SIZE(low)) {
		n->num[SIZE(n)] = low->num[SIZE(n)];
		INC_SIZE(n);
	}
	while (SIZE(n) < SIZE(low) + SIZE(high)) {
		n->num[SIZE(n)] = high->num[SIZE(n) - SIZE(low)];
		INC_SIZE(n);
	}
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

t_bn 	*bn_init_size(uint64_t size)
{
	t_bn 	*n = NULL;
	
	n = (t_bn*)malloc(sizeof(t_bn));
	if (n == NULL)
		return (NULL);
	n->alloc = size / 64 + 2;
	n->size = 0;
	n->num = (uint64_t*)malloc(sizeof(uint64_t) * n->alloc);
	if (n->num == NULL)
		return (NULL);
	for (int64_t i = 0; i < n->alloc; i++)
		n->num[i] = 0;
	return (n);
}

t_bn	*bn_init(void)
{
	return (bn_init_size(1));
}

int 	bn_set_random(t_bn *n, int64_t size)
{
	int 		to_read = 0;
	int 		fd = open("/dev/random", O_RDONLY);
	uint32_t 	bits;

	if (fd < 0)
		return 1;
	if (size % 8)
		to_read++;
	to_read += size / 8;
	if (read(fd, (void*)n->num, to_read) < 0)
		return 1;
	n->size = size / 64;
	if (size % 64 && n->num[n->size])
		n->size++;
	bits = bn_get_bit_number(n);
	if (bits > size)
		bn_shift_right(n, bits - size);
	else if (bits < size)
		bn_shift_left(n, size - bits);
	close(fd);
	return 0;
}

void	display_bn(t_bn n)
{
	if (n.size == 0)
		printf("0");
	else
	{	
		for (int64_t i = n.size - 1; i > -1; i--)
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
	printf("Size = %llu\n", n.size);
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

int		bn_realloc(t_bn *n)
{
	uint64_t	*tmp;

	tmp = n->num;
	n->num = (uint64_t*)malloc(sizeof(uint64_t) * n->alloc * 2);
	if (n->num == NULL)
		return (1);
	memset(n->num, 0, n->alloc * 2 * sizeof(uint64_t));
	memcpy((void*)n->num, (void*)tmp, SIZE(n) * 8);
	n->alloc *= 2;
	free(tmp);
	return (0);
}

int 	bn_realloc_size(t_bn *n, int64_t new_alloc)
{
	uint64_t 	*tmp;

	tmp = n->num;
	n->num = (uint64_t*)malloc(sizeof(uint64_t) * new_alloc);
	if (n->num == NULL)
		return (1);
	memset(n->num, 0, new_alloc * sizeof(uint64_t));
	memcpy((void*)n->num, (void*)tmp, SIZE(n) * 8);
	n->alloc = new_alloc;
	free(tmp);
	return (0);
}

int 	bn_realloc_size_zero(t_bn *n, int64_t new_alloc)
{
	free(n->num);
	n->num = (uint64_t*)malloc(sizeof(uint64_t) * new_alloc);
	if (n->num == NULL)
		return (1);
	for (int64_t i = 0; i < new_alloc; i++)
		n->num[i] = 0;
	n->alloc = new_alloc;
	return (0);
}

void	bn_add_ui(t_bn *n, uint64_t ui)
{
	if (n->size == 0)
	{
		n->num[SIZE(n)] = ui;
		INC_SIZE(n);
		return ;
	}
	for (int64_t i = 0; i < SIZE(n); i++)
	{
		if (n->num[i] + ui < n->num[i])
		{
			n->num[i] += ui;
			ui = 1;
			if (SIZE(n) > n->alloc)
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
		n->num[SIZE(n)] = ui;
}

int 	bn_cmp(t_bn *a, t_bn *b) // Return 1 if a > b, 0 if a == b, -1 if a < b
{
	if (a->size > b->size)
		return (1);
	else if (a->size < b->size)
		return (-1);
	for (int64_t i = SIZE(a) - 1; i >= 0; i--)
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
	if (n->size > 1)
		return (1);
	if (n->size == 0 && ui == 0)
		return (0);
	if (n->size == 1 && n->num[0] == ui)
		return (0);
	if (n->size == 1 && n->num[0] > ui)
		return (1);
	if (n->size == 1 && n->num[0] < ui)
		return (-1);
	return (1);
}

t_bn 	*bn_clone(t_bn *a) // Returns a freshly allocated copy of bn a
{
	t_bn 		*n;
	int64_t 	i;

	n = (t_bn*)malloc(sizeof(t_bn));
	if (n == NULL)
		return (NULL);
	n->num = (uint64_t*)malloc(sizeof(uint64_t) * (a->alloc + 1));
	if (n->num == NULL)
		return (NULL);
	n->size = a->size;
	n->alloc = a->alloc;
	for (i = 0 ; i < SIZE(n); i++)
		n->num[i] = a->num[i];
	while (i < n->alloc)
		n->num[i++] = 0;
	return (n);
}

void	bn_copy(t_bn *a, t_bn *b) // Copy value of b into a. Reallocate a if necessary
{
	int64_t i;

	if (a->alloc < SIZE(b))
		bn_realloc_size_zero(a, SIZE(b));
	for (i = 0 ; i < SIZE(b); i++)
		a->num[i] = b->num[i];
	while (i < a->alloc)
		a->num[i++] = 0;
	a->size = b->size;
}

void	bn_add(t_bn *res, t_bn *op1, t_bn *op2)
{
	int64_t 	carry = 0;
	int64_t		max = MAX(SIZE(op1), SIZE(op2));
	t_bn 		*cres;
	int64_t		i, tmp;

	if (op1->size == 0 || op2->size == 0)
	{
		bn_copy(res, (SIZE(op1) > SIZE(op2)) ? op1 : op2);
		return ;
	}
	if (res->alloc < SIZE(op1) + SIZE(op2))
		bn_realloc_size(res, SIZE(op1) + SIZE(op2) + 1);
	if ((cres = bn_clone(res)) == NULL)
		return ;
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


void	bn_shift_right(t_bn *n, uint64_t shift)
{
	uint64_t	lost, previous_lost;
	int64_t 	q, r, i;

	q = shift / 64;
	r = shift % 64;
	previous_lost = lost = 0;
	if (q >= SIZE(n))
		bn_set_zero(n);
	else if (shift)
	{
		for (i = 0; i < SIZE(n) && q > 0; i++)
		{
			if (i < q)
				n->num[i] = n->num[i + q];
			else
				n->num[i] = 0;
		}
		n->size -= q;
		for (i = SIZE(n) - 1 ; i >= 0 ; i--)
		{
			previous_lost = lost;
			lost = n->num[i] << (64 - r) >> (64 - r);
			n->num[i] >>= r;
			n->num[i] |= previous_lost << (64 - r);
			if (i == n->size - 1 && n->num[i] == 0)
				DEC_SIZE(n);
		}
	}
}

void	bn_shift_left(t_bn *n, uint64_t shift)
{
	uint64_t	lost, previous_lost;
	int64_t 	q, r, i;

	if (n->size == 0)
		return ;
	q = shift / 64;
	r = shift % 64;
	previous_lost = lost = 0;
	if (q >= SIZE(n))
		bn_set_zero(n);
	else if (shift)
	{
		for (i = n->size - 1; i >= 0 && q > 0; i--)
		{
			if (i >= q)
				n->num[i] = n->num[i - q];
			else
				n->num[i] = 0;
		}
		for (i = 0 ; i < SIZE(n); i++)
		{
			previous_lost = lost;
			lost = n->num[i] >> (64 - r);
			n->num[i] <<= r;
			n->num[i] |= previous_lost;
		}
		n->num[i] |= lost;
		if (n->num[i])
			INC_SIZE(n);
	}
}

void	print_stats(t_bn n)
{
	printf("%llu\n",n.size);
	printf("%llu\n", n.alloc);
}

void	bn_mul(t_bn *res, t_bn *m, t_bn *q)
{
	t_bn 		*a = bn_init_size(MAX(m->size, q->size) * 64 + 1);
	t_bn		*cq = bn_clone(q);
	t_bn 		*cm = bn_clone(m);
	int64_t 	orig_q_size = SIZE(q);

	if (!a || !cq || !cm)
	{
		bn_clears(3, &a, &cq, &cm);
		return ;
	}
	for (int64_t i = 0; i < orig_q_size * 64; i++)
	{
		if (IS_ODD(cq->num[0]))
			bn_add(a, a, cm);
		bn_shift_right(cq, 1);
		cq->size = orig_q_size;
		if (IS_ODD(a->num[0]))
			cq->num[orig_q_size - 1] |= 0x8000000000000000;
		bn_shift_right(a, 1);
	}
	bn_concat(res, cq, a);
	bn_clears(3, &a, &cq, &cm);
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

void	bn_clears(int num, ...)
{
	va_list args;

	va_start(args, num);
	for (int i = 0; i < num; i++)
	{
		t_bn **n = va_arg(args, t_bn **);
		bn_clear(n);
	}
	va_end(args);
}

int		bn_pow(t_bn *n, t_bn *b, t_bn *exp)
{
	t_bn	*cb = bn_clone(b);
	t_bn	*cexp = bn_clone(exp);

	if (cb == NULL || cexp == NULL)
		return (1);
	bn_set_zero(n);
	bn_add_ui(n, 1);
	while (SIZE(cexp) > 0 || cexp->num[0] > 0)
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

void 	bn_mod_pow(t_bn *res, t_bn *b, t_bn *exp, t_bn *mod)
{
	t_bn	*cb = bn_clone(b);
	t_bn	*cexp = bn_clone(exp);
	t_bn	*cmod = bn_clone(mod);;

	bn_set_zero(res);
	bn_add_ui(res, 1);
	bn_mod(cb, cb, cmod);
	while (bn_cmp_ui(cexp, 0))
	{
		if (IS_ODD(cexp->num[0]))
		{
			bn_mul(res, res, cb);
			bn_mod(res, res, cmod);
		}
		bn_shift_right(cexp, 1);
		bn_mul(cb, cb, cb);
		bn_mod(cb, cb, cmod);
	}
	bn_clears(3, &cb, &cexp, &cmod);
}

void	bn_sub_ui(t_bn *res, t_bn *a, uint64_t ui)
{
	t_bn		*ca = NULL;
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
	t_bn 		*ca = NULL;
	t_bn 		*cb = NULL;
	int64_t		j;

	if (bn_cmp(a, b) < 0)
		return ;
	ca = bn_clone(a);
	cb = bn_clone(b);
	bn_set_ui(res, 0);
	for (int64_t i = 0; i < SIZE(ca); i++)
	{
		if (i > SIZE(cb) - 1)
			res->num[i] = ca->num[i];
		else
			res->num[i] = ca->num[i] - cb->num[i];
		if (cb->num[i] && res->num[i] >= ca->num[i])
		{
			j = i + 1;
			while (ca->num[j] == 0 && j < SIZE(ca) - 2)
				ca->num[j++] = ULLONG_MAX;
			ca->num[j] -= 1;
		}
	}
	for (int64_t i = res->alloc - 1; i >= 0; i--)
	{
		if (res->num[i] != 0)
		{
			res->size = i + 1;
			break;
		}
	}
	bn_clears(2, &ca, &cb);
}

void	bn_mod(t_bn *r, t_bn *n, t_bn *d)
{
	t_bn	*cn = NULL;
	t_bn	*cd = NULL;

	if (bn_cmp(n, d) < 0)
	{
		if (r != n)
			bn_copy(r, n);
		return;
	}
	cn = bn_clone(n);
	cd = bn_clone(d);
	bn_set_zero(r);
	for (int64_t i = SIZE(cn) * 64 - 1; i >= 0; i--)
	{
		bn_shift_left(r, 1);
		r->num[0] ^= bn_get_bit(cn, i);
		if (SIZE(r) == 0 && r->num[0] > 0)
			INC_SIZE(r);
		if (bn_cmp(r, cd) >= 0)
			bn_sub(r, r, cd);
	}
	bn_clears(2, &cn, &cd);
}

void	bn_div(t_bn *q, t_bn *r, t_bn *n, t_bn *d)
{
	t_bn	*cn = bn_clone(n);

	bn_set_zero(r);
	bn_set_zero(q);
	for (int64_t i = n->size * 64 - 1; i >= 0; i--)
	{
		bn_shift_left(r, 1);
		r->num[0] ^= bn_get_bit(n, i);
		if (r->size == 0 && r->num[0] > 0)
			r->size++;
		if (bn_cmp(r, d) >= 0)
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

void	bn_swap(t_bn *a, t_bn *b)
{
	uint64_t	*n;
	int64_t		tmp;

	n = a->num;
	a->num = b->num;
	b->num = n;

	tmp = a->size;
	a->size = b->size;
	b->size = tmp;

	tmp = a->alloc;
	a->alloc = b->alloc;
	b->alloc = tmp;
}