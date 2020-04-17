
#include "ft_ssl.h"

void		reset_des(t_des *des)
{
	if (des->input_file)
		close(des->fd[IN]);
	if (des->output_file)
		close(des->fd[OUT]);
	ft_strdel(&des->password);
	ft_strdel((char**)&des->salt);
	ft_memdel((void**)&des->base64);
	ft_memdel((void**)&des);
}