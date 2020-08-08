#include "bn.h"

int		bn_copy(t_bn *a, t_bn *b) // Copy value of b into a. Reallocate a if necessary
{
	int64_t i;

	if (a->alloc < SIZE(b))
	{
		if (bn_realloc_size_zero(a, SIZE(b)))
			return (1);
	}
	i = -1;
	while (++i < SIZE(b))
		a->num[i] = b->num[i];
	// while (i < a->alloc)
	// 	a->num[i++] = 0;
	a->size = b->size;
	return (0);
}