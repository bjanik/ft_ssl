#include "bn.h"
#include "ft_ssl.h"

void	flag_modulus(t_bn *n, int fd)
{
	ft_putstr_fd("Modulus=", fd);
	display_bn(n);
}