#include "ft_ssl.h"

t_rsautl	*rsautl_init(void)
{
	t_rsautl 	*rsautl;

	rsautl = (t_rsautl*)malloc(sizeof(t_rsautl));
	if (rsautl == NULL)
		return (NULL);
	rsautl->in = NULL;
	rsautl->out = NULL;
	rsautl->inkey = NULL;
	rsautl->fd[IN] = STDIN_FILENO;
	rsautl->fd[OUT] = STDOUT_FILENO;
	rsautl->opts = 0;
	rsautl->des = NULL;
	ft_memset((void*)(&rsautl->rsa_data), 0x0, sizeof(t_rsa_data));
	return (rsautl);
}