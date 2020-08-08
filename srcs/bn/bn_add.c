#include "bn.h"

static void	add_loops(t_bn *res, t_bn *max, t_bn *min)
{
	uint64_t 	tmp;
	int64_t 	carry;
	int64_t 	i;

	i = -1;
	carry = 0;
	while (++i < SIZE(min))
	{
		tmp = max->num[i] + min->num[i] + carry;
		if (carry || min->num[i])
			carry = (tmp <= max->num[i]) ? 1 : 0;
		res->num[i] = tmp;
		(i >= SIZE(res)) ? INC_SIZE(res) : 0;
	}
	while (i < SIZE(max))
	{
		tmp = max->num[i] + carry;
		carry = (tmp < max->num[i]) ? 1 : 0;
		res->num[i] = tmp;
		(i >= SIZE(res)) ? INC_SIZE(res) : 0;
		i++;
	}
	res->num[i] = carry;
	(i == SIZE(res) && carry) ? INC_SIZE(res) : 0;
}

int				bn_add(t_bn *res, t_bn *op1, t_bn *op2)
{
	t_bn 		*max;
	t_bn 		*min;

	max = op1;
	min = op2;
	if (SIZE(op1) < SIZE(op2))
	{
		max = op2;
		min = op1;
	}
	add_loops(res, max, min);
	return (0);
}

int		bn_add_ui(t_bn *n, uint64_t ui)
{
	int64_t i;

	if (n->size == 0)
	{
		n->num[n->size++] = ui;
		return (0);
	}
	i = -1;
	while (++i < SIZE(n))
	{
		if (n->num[i] + ui < n->num[i])
		{
			n->num[i] += ui;
			ui = 1;
			(SIZE(n) > n->alloc) ? bn_realloc(n) : 0;
		}
		else
		{
			n->num[i] += ui;
			ui = 0;
			break;
		}
	}
	(ui == 1) ?	n->num[SIZE(n)] = ui : 0;
	return (0);
}