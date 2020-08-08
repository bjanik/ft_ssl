#include "ft_ssl.h"

t_genrsa	*genrsa_init(void)
{
	t_genrsa 	*genrsa;

	if ((genrsa = ft_malloc(sizeof(t_genrsa))) == NULL)
		return (NULL);
	genrsa->out = NULL;
	genrsa->fd[IN] = STDIN_FILENO;
	genrsa->fd[OUT] = STDOUT_FILENO;
	genrsa->numbits = 64;
	return (genrsa);
}