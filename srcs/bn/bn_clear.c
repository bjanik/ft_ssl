#include <stdarg.h>
#include <stdlib.h>
#include "bn.h"

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