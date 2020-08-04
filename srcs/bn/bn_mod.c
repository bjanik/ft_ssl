#include "bn.h"

void	bn_mod(t_bn *r, t_bn *n, t_bn *d)
{
	t_bn	*cn;
	t_bn 	*cr;

	if (bn_cmp(n, d) < 0)
	{
		if (r != n)
			bn_copy(r, n);
		return;
	}
	cn = bn_clone(n);
	cr = bn_clone(d);
	bn_set_zero(r);
	for (int64_t i = SIZE(cn) * 64 - 1; i >= 0; i--)
	{
		bn_shift_left(r, 1);
		r->num[0] ^= bn_get_bit(cn, i);
		if (SIZE(r) == 0 && r->num[0] > 0)
			INC_SIZE(r);
		if (SIZE(r) > r->alloc)
			bn_realloc(r);
		bn_copy(cr, r);
		if (bn_cmp(r, d) >= 0)
			bn_sub(r, cr, d);
	}
	bn_clears(2, &cn, &cr);
}