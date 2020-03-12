#include "bn.h"
#include "ft_ssl.h"

t_rsa		*rsa_init(void)
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

static char	*get_data(int fd[])
{
	char			*line = NULL;
	char			*data = NULL;
	int 			ret = 0;

	while ((ret = get_next_line(fd[IN], &line)) > 0)
	{
		if (ft_strcmp(line, PEM_HEADER) == 0)
		{
			if ((data = ft_strnew(1)) == NULL)
				return (NULL);
			continue ;
		}
		if (ft_strcmp(line, PEM_FOOTER) == 0)
			break ;
		if (data)
			data = ft_strnjoin(data, 1, line);
		ft_strdel(&line);
	}
	ft_strdel(&line);
	if (data == NULL)
		return (NULL);
	if (ret < 0)
		return (NULL);
	return (data);
}

int 		rsa_command_run(t_rsa *rsa)
{
	char			*data = NULL;

	if ((data = get_data(rsa->fd)) == NULL)
		return (1);
	pem_decode(rsa, data);
}