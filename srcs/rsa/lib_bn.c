#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MIN(x, y) (x < y) ? x : y
#define MAX(x, y) (x > y) ? x : y

typedef struct 	big_num
{
	uint64_t	*num; // Array of long long int representing the big num
	uint64_t	size; // Biggest index of num array not set to 0. For instance, if num == ULLONG_MAX + 1, size is 2. If num == 1, size is 1. If num == 0, size is 0
	uint64_t	alloc; // Capacity of array num. Alloc must always be >= size
}				t_bn;


t_bn 	*bn_init_size(size_t size)
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

void	bn_set_zero(t_bn *n)
{
	for (uint64_t i = 0; i < n->size; i++)
		n->num[i] = 0;
	n->size = 0;
}

int 	bn_set_random(t_bn *n, size_t size)
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

int		bn_realloc(t_bn *n)
{
	uint64_t	*tmp;

	tmp = n->num;
	n->num = (uint64_t*)malloc(sizeof(uint64_t) * n->alloc * 2);
	memset(n->num, 0, n->alloc * 2 * sizeof(uint64_t));
	if (n->num == NULL)
		return (1);
	memcpy((void*)tmp, (void*)n->num, n->size * 8);
	free(n->num);
	n->num = tmp;
	n->alloc *= 2;
	return (0);
}

void	bn_add_ui(t_bn *n, uint64_t ui)
{
	if (n->size == 0)
	{
		n->num[0] = ui;
		n->size++;
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

void 	bn_clone(t_bn *a, t_bn b) // Copy value of b into a. Assume enough sapce is allocated in a to hold b
{
	uint64_t i;

	for (i = 0 ; i < b.size; i++)
		a->num[i] = b.num[i];
	while (i < a->alloc)
		a->num[i++] = 0;
	a->size = b.size;
}

void	bn_add(t_bn *res, t_bn op1, t_bn op2)
{
	uint8_t 	carry = 0;
	uint64_t	min = MIN(op1.size, op2.size);
	uint64_t	max = MAX(op1.size, op2.size);
	uint64_t	i;
	t_bn		*t;

	if (op1.size == 0 && op2.size == 0)
	{
		bn_set_zero(res);
		return ;
	}
	if (op1.size == 0 || op2.size == 0)
	{
		bn_clone(res, (op1.size > op2.size) ? op1 : op2);
		return ;
	}
	if (res->alloc <= max)
		res->size = max;
	for (i = 0 ; i < min; i++)
	{
		res->num[i] = op1.num[i] + op2.num[i] + carry;
		carry = (op1.num[i] + op2.num[i] + carry < op1.num[i]) ? 0 : 0;
		if (i > res->size)
			res->size++;
	}
	(carry) ? res->size++ : 0;
	t = (op1.size >= i) ? &op1 : &op2;
	while (i < t->size)
	{
		if (i > res->size)
			res->size++;
		if (res->num[i] + t->num[i] + carry < res->num[i])
			carry = 1;
		i++;
	}
	if (carry)
	{
		res->num[i] = 1;
		if (i == res->size)
			res->size++;
	}
}

void 	power_of_two(t_bn *n, unsigned int pow)
{
	uint64_t 	limb;

	limb = pow / 64;
	while (limb >= n->alloc)
	{
		printf("REALLOC\n");
		bn_realloc(n);
	}
	bn_set_zero(n);
	n->num[limb] = 1;
	n->num[limb] <<= pow % 64;
	n->size = limb + 1;
}

void	bn_mul_ui(t_bn *res, t_bn op, uint64_t ui)
{


}

void	bn_mul(t_bn *res, t_bn op1, t_bn op2)
{
	t_bn *a = bn_init_size(MAX(op1.size, op2.size) * 64 + 1);
	uint8_t		carry = 0;

	// display_bn(op1);
	bn_add(a, *a, op1);
	// display_bn(*a);

	// printf("%llu\n", op1.size);
	// printf("%llu\n", op2.size);
	// printf("%llu\n", a->size);
	// printf("%llu\n", a->alloc);

}

void	bn_shift_right(t_bn *n, uint64_t shift)
{
	int 		q, r, i;
	uint64_t	left;

	q = shift / 64;
	r = shift % 64;
	if (q >= n->size)
		bn_set_zero(n);
	else
	{
		for (i = 0; i < n->size && q > 0; i++)
		{
			if (i < q)
				n->num[i] = n->num[i + q];
			else
				n->num[i] = 0;
		}
		n->size -= q;
		for (i = n->size - 1 ; i >= 0 ; i--)
		{
			left = (n->num[i] << (64 - r)) >> (64 - r);
			n->num[i] = n->num[i] >> r;
			if (i < n->size - 1)
				n->num[i] |= left << (64 - r);
		}
	}
}

// void	bn_shift_left(t_bn *n, uint64_t shift)
// {
	
// }

int main()
{
	t_bn 	*n;
	t_bn 	*p;
	t_bn 	*q;
	t_bn 	*z;
	t_bn	*x;

	n = bn_init_size(1024);
	p = bn_init_size(1024);
	q = bn_init_size(1024);
	z = bn_init_size(1024);
	x = bn_init_size(1024);

	bn_set_random(n, 240);
	bn_set_random(p, 512);
	bn_set_random(q, 256);
	bn_add_ui(x, ULLONG_MAX);
	// bn_add(x, *x, *n);
	// printf("%llu\n", x->size);
	x->num[0] = x->num[1] = x->num[2] = ULLONG_MAX;
	x->size = 3;
	display_bn(*x);
	bn_shift_right(x, 63);
	display_bn(*x);
	// bn_shift_right(x, 128);
	// bn_shift_right(x, 64);
	// display_bn(*x);


	// bn_clone(x, *n);
	// display_bn(*n);
	// display_bn(*x);



	// bn_add(x, *p, *q);

	// bn_mul(p, *q, *n);

	return 0;
}