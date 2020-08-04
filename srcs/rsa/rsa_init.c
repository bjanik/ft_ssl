#include "ft_ssl.h"

t_rsa				*rsa_init(void)
{
	t_rsa	*rsa;

	if ((rsa = (t_rsa*)malloc(sizeof(t_rsa))) == NULL)
		return (NULL);
	rsa->inform = NULL;
	rsa->outform = NULL;
	rsa->in = NULL;
	rsa->out = NULL;
	rsa->fd[IN] = STDIN_FILENO;
	rsa->fd[OUT] = STDOUT_FILENO;
	rsa->passin = NULL;
	rsa->passout = NULL;
	rsa->opts = 0;
	rsa->des = NULL;
	return (rsa);
}