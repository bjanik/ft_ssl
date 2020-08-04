#include "bn.h"

static int	check_if_zero(t_bn *q, t_bn *r, t_bn *n, t_bn *d)
{
	if (bn_cmp(n, d) < 0)
	{
		bn_copy(r, n);
		bn_set_zero(q);
		return (0);
	}
	return (1);
}

void		bn_div(t_bn *q, t_bn *r, t_bn *n, t_bn *d)
{
	t_bn	*cr;
	int64_t	i;

	if (check_if_zero(q, r, n, d) == 0)
		return;
	cr = bn_clone(r);
	bn_set_zero(r);
	bn_set_zero(q);
	i = bn_get_bit_number(n);
	while (--i >= 0)
	{
		bn_shift_left(r, 1);
		r->num[0] ^= bn_get_bit(n, i);
		if (SIZE(r) == 0 && r->num[0] > 0)
			INC_SIZE(r);
		bn_copy(cr, r);
		if (bn_cmp(cr, d) >= 0)
		{
			bn_sub(r, cr, d);
			bn_set_bit(q, i);
		}
	}
	bn_clear(&cr);
}