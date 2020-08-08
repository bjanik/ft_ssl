#include "ft_ssl.h"

int 			rsa_input_file(t_rsa *rsa)
{
	if (rsa->in)
	{
		if ((rsa->fd[IN] = open(rsa->in, O_RDONLY, 0644)) < 0)
			return (1);
	}
	return (0);
}

int 			rsa_output_file(t_rsa *rsa)
{
	if (rsa->out)
	{
		if ((rsa->fd[OUT] = open(rsa->out, O_CREAT | O_TRUNC | O_WRONLY, 0644)) < 0)
			return (1);
	}
	return (0);
}