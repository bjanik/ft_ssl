#include "bn.h"
#include "libft.h"

t_bn 	*bn_init_size(uint64_t size)
{
	t_bn 	*n;
	int64_t i;
	
	n = (t_bn *)ft_malloc(sizeof(t_bn));
	if (n == NULL)
		return (NULL);
	n->alloc = size / 64 + 2;
	n->size = 0;
	n->num = (uint64_t*)ft_malloc(sizeof(uint64_t) * n->alloc);
	if (n->num == NULL)
	{
		ft_memdel((void**)&n);
		return (NULL);
	}
	i = -1;
	while (++i < n->alloc)
		n->num[i] = 0;
	return (n);
}

t_bn	*bn_init(void)
{
	return (bn_init_size(1));
}