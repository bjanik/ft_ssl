
#include "ft_ssl.h"

void		reset_des(t_ssl_command *cmd)
{
	if (cmd->des->input_file)
		close(cmd->des->fd[IN]);
	if (cmd->des->output_file)
		close(cmd->des->fd[OUT]);
	ft_strdel(&cmd->des->password);
	ft_strdel((char**)&cmd->des->salt);
	ft_memdel((void**)&cmd->des->base64);
	ft_memdel((void**)&cmd->des);
}