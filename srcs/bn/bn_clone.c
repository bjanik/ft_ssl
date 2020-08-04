#include "libft.h"
#include "bn.h"

t_bn 	*bn_clone(t_bn *a)
{
	t_bn 		*n;
	int64_t 	i;

	n = (t_bn*)ft_malloc(sizeof(t_bn));
	if (n == NULL)
		return (NULL);
	n->num = (uint64_t*)ft_malloc(sizeof(uint64_t) * (a->alloc + 1));
	if (n->num == NULL)
	{
		free(n);
		return (NULL);
	}
	n->size = a->size;
	n->alloc = a->alloc;
	i = -1;
	while (++i < SIZE(n))
		n->num[i] = a->num[i];
	while (i < n->alloc)
		n->num[i++] = 0;
	return (n);
}