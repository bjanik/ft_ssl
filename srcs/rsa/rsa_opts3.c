#include "ft_ssl.h"

int 	set_pubin(char **argv, t_rsa *rsa, int *index)
{
	(void)argv;
	(void)index;
	rsa->opts |= RSA_PUBIN;
	return (0);
}

int 	set_pubout(char **argv, t_rsa *rsa, int *index)
{
	(void)argv;
	(void)index;
	rsa->opts |= RSA_PUBOUT;
	return (0);
}