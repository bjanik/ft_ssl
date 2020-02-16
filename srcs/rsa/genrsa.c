#include "ft_ssl.h"

t_genrsa	*genrsa_init(void)
{
	t_genrsa 	*genrsa;

	if ((genrsa = malloc(sizeof(t_genrsa))) == NULL)
		return (NULL);
	genrsa->in = NULL;
	genrsa->out = NULL;
	genrsa->fd[IN] = STDIN_FILENO;
	genrsa->fd[OUT] = STDOUT_FILENO;
	genrsa->rand_file = NULL;
	genrsa->numbits = 0;
	return (genrsa);
}