#include "ft_ssl.h"
#include "fcntl.h"
#include "unistd.h"


struct			s_rsa_opts
{
	char		*opt;
	int			(*opt_f)(char **argv, t_rsa *rsa, int *index);
};



int 		set_inform(char **argv, t_rsa *rsa, int *index)
{
	(*index)++;
	rsa->inform = argv[*index];
	return (0);
}

int 		set_outform(char **argv, t_rsa *rsa, int *index)
{
	(*index)++;
	rsa->outform = argv[*index];
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
	if (rsa->opts & RSA_DES3)
		rsa->opts &= ~RSA_DES3;
	return (0);
}

int 	set_des3_encryption(char **argv, t_rsa *rsa, int *index)
{
	(void)argv;
	(void)index;
	rsa->opts |= RSA_DES3;
	if (rsa->opts & RSA_DES)
		rsa->opts &= ~RSA_DES;
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
		if (read(STDIN_FILENO, &rsa->passin, 64) < 0)
			return (1);
	}
	else if (ft_strcmp(splitted_str[0], "pass") == 0)
	{
		if ((rsa->passin = ft_strdup(splitted_str[1])) == NULL)
			return (1);
	}
	else if (ft_strcmp(splitted_str[0], "env") == 0)
	{
		if ((rsa->passin = ft_strdup(getenv(splitted_str[1]))) == NULL)
			return (1);
	}
	else if (ft_strcmp(splitted_str[0], "file") == 0)
	{
		if ((fd = open(splitted_str[1], O_RDONLY)) < 0)
			return (1);
		if (get_next_line(fd, &rsa->passin) < 0)
			return (1);
		close(fd);
	}
	else
		return (1);
	ft_free_string_tab(&splitted_str);
	return (0);
}

int 	set_password_output(char **argv, t_rsa *rsa, int *index)
{
	(*index)++;
	if ((rsa->passout = argv[*index]) == NULL)
		return (1);
	return (0);
}

const struct s_rsa_opts  g_rsa_opts[] = {
	{"-inform", set_inform},
	{"-outform", set_outform},
	{"-in", set_in_file},
	{"-out", set_out_file},
	{"-passin", set_password_rsa},
	{"-passout", set_password_output},
	{"-des", set_des_encryption},
	{"-des3", set_des3_encryption},
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
		if ((rsa->fd[IN] = open(rsa->in, O_RDONLY, 0644)) < 0)
			return (1);
	}
	if (rsa->out)
	{
		if ((rsa->fd[OUT] = open(rsa->out, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0)
			return (1);	
	}
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
			if (ft_strcmp(g_rsa_opts[j].opt, argv[i]) == 0)
			{
				if (g_rsa_opts[j].opt_f(argv, rsa, &i))
					return (1);
				break ;	
			}
		}
		if (g_rsa_opts[j].opt == NULL)
		{
			ft_putstr_fd("Unknown option: ", STDERR_FILENO);
			ft_putendl_fd(argv[i], STDERR_FILENO);
			return (1);
		}
	}
	return (finalize_rsa_opts(rsa));
}
