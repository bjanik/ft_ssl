#include "ft_ssl.h"


struct			s_rsa_opts
{
	char		*opt;
	int			(*opt_f)(char **argv, t_rsa *rsa, int *index);
};


const struct s_rsa_opts  g_rsa_opts[] = {
	{"-inform", set_inform},
	{"-outform", set_outform},
	{"-in", set_in_file},
	{"-out", set_out_file},
	{"-passin", set_password_rsa},
	{"-passout", set_password_output},
	{"-des", set_des_encryption},
	{"-text", set_text},
	{"-noout", set_noout},
	{"-modulus", set_modulus},
	{"-check", set_check},
	{"-pubin", set_pubin},
	{"-pubout", set_pubout},
	{NULL, NULL}
};


static int 	finalize_rsa_opts(t_rsa *rsa)
{
	if (rsa->in)
	{
		if ((rsa->fd[IN] = open(rsa->in, O_RDONLY)) < 0)
			return (1);
	}
	if (rsa->out)
	{
		if ((rsa->fd[OUT] = open(rsa->out, O_WRONLY)) < 0)
			return (1);	
	}
	return (0);
}

int 		set_inform(char **argv, t_rsa *rsa, int *index)
{
	rsa->inform = argv[++(*index)];
	return (0);
}

int 		set_outform(char **argv, t_rsa *rsa, int *index)
{
	rsa->outform = argv[++(*index)];
	return (0);
}

int		set_in_file(char **argv, t_rsa *rsa, int *index)
{
	rsa->in = argv[++(*index)];
	if (rsa->in == NULL)
		return (1);
	return (0);
}

int		set_out_file(char **argv, t_rsa *rsa, int *index)
{
	rsa->out = argv[++(*index)];
	if (rsa->in == NULL)
		return (1);
	return (0);
}

int 	set_des_encryption(char **argv, t_rsa *rsa, int *index)
{
	(void)argv;
	(void)index;
	rsa->opts |= RSA_DES;
	return (0);
}

int 	set_check(char **argv, t_rsa *rsa, int *index)
{
	(void)argv;
	(void)index;
	rsa->opts |= RSA_CHECK;
	return (0);
}

int 	set_text(char **argv, t_rsa *rsa, int *index)
{
	(void)argv;
	(void)index;
	rsa->opts |= RSA_TEXT;
	return (0);
}

int 	set_noout(char **argv, t_rsa *rsa, int *index)
{
	(void)argv;
	(void)index;
	rsa->opts |= RSA_NOOUT;
	return (0);
}

int 	set_modulus(char **argv, t_rsa *rsa, int *index)
{
	(void)argv;
	(void)index;
	rsa->opts |= RSA_MODULUS;
	return (0);
}

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

int 	set_password_rsa(char **argv, t_rsa *rsa, int *index)
{
	char	**splitted_str = NULL;
	int		fd;

	if ((splitted_str = ft_strsplit(argv[++(*index)], ':')) == NULL || !splitted_str[1])
	{
		ft_free_string_tab(&splitted_str);
		return (1);
	}
	if (ft_strcmp(argv[*index], "stdin") == 0)
	{
		read(STDIN_FILENO, &rsa->passin, 64);
	}
	else if (ft_strcmp(splitted_str[0], "pass"))
	{
		if ((rsa->passin = ft_strdup(splitted_str[1])) == NULL)
			return (1);
	}
	else if (ft_strcmp(splitted_str[0], "env"))
	{
		if ((rsa->passin = ft_strdup(getenv(splitted_str[1]))) == NULL)
			return (1);
	}
	else if (ft_strcmp(splitted_str[0], "file"))
	{
		if ((fd = open(splitted_str[1], O_RDONLY)) < 0)
			return (1);
		get_next_line(fd, &rsa->passin);
		close(fd);
	}
	else
		return (1);
	ft_free_string_tab(&splitted_str);
	return (0);
}

int 	set_password_output(char **argv, t_rsa *rsa, int *index)
{
	if ((rsa->passout = argv[++(*index)]) == NULL)
		return (1);
	return (0);
}

int 	rsa_opts(char **argv, t_rsa *rsa)
{
	int i, j;

	i = 1;
	while (argv[++i])
	{
		j = -1;
		while (g_rsa_opts[++j].opt)
		{
			if (ft_strcmp(g_rsa_opts[++j].opt, argv[i]) == 0)
			{
				if (g_rsa_opts[j].opt_f(argv, rsa, &i))
					return (1);
				break ;	
			}
		}
	}
	return (finalize_rsa_opts(rsa));
}
