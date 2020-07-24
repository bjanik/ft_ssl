#include "bn.h"
#include "ft_ssl.h"

void	flag_modulus(t_bn *n, int fd)
{
	ft_dprintf(fd, "Modulus=");
	display_bn(n);
}