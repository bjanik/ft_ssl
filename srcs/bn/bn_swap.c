#include "bn.h"

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