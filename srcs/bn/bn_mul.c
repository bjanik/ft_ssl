#include "bn.h"

static int 	bn_concat(t_bn *n, t_bn *low, t_bn *high)
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
	return (0);
}

void		bn_mul(t_bn *res, t_bn *m, t_bn *q)
{
	t_bn 		*a;
	t_bn		*cq;
	t_bn 		*cm;
	t_bn		*ca;
	int64_t 	i;

	a = bn_init_size(MAX(m->size, q->size) * 64 + 1);
	cq = bn_clone(q);
	cm = bn_clone(m);
	ca = bn_clone(a);
	i = -1;
	while (++i < SIZE(q) * 64)
	{
		bn_copy(ca, a);
		if (IS_ODD(cq->num[0]))
			bn_add(a, ca, cm);
		bn_shift_right(cq, 1);
		cq->size = SIZE(q);
		if (IS_ODD(a->num[0]))
			cq->num[SIZE(q) - 1] |= 0x8000000000000000;
		bn_shift_right(a, 1);
	}
	bn_concat(res, cq, a);
	bn_clears(4, &a, &cq, &cm, &ca);
}