#include "ft_ssl.h"
#include "fcntl.h"
#include "unistd.h"


struct			s_rsautl_opts
{
	char		*opt;
	int			(*opt_f)(char **argv, t_rsautl *rsautl, int *index);
};

static int 	set_rsautl_in(char **argv, t_rsautl *rsautl, int *index)
{
	(*index)++;
	if ((rsautl->in = argv[*index]) == NULL)
	{
		ft_putendl_fd("ft_ssl: Missing input file", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int 	set_rsautl_out(char **argv, t_rsautl *rsautl, int *index)
{
	(*index)++;
	if ((rsautl->out = argv[*index]) == NULL)
	{
		ft_putendl_fd("ft_ssl: Missing output file", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int 	set_inkey_file(char **argv, t_rsautl *rsautl, int *index)
{
	(*index)++;
	if ((rsautl->inkey = argv[*index]) == NULL)
	{
		ft_putendl_fd("ft_ssl: Missing inkey file", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int 	set_hexdump(char **argv, t_rsautl *rsautl, int *index)
{
	(void)argv;
	(void)index;
	rsautl->opts |= RSAUTL_HEXDUMP;
	return (0);
}

static int 	set_encrypt(char **argv, t_rsautl *rsautl, int *index)
{
	(void)argv;
	(void)index;
	rsautl->opts |= RSAUTL_ENCRYPT;
	return (0);
}

static int 	set_decrypt(char **argv, t_rsautl *rsautl, int *index)
{
	(void)argv;
	(void)index;
	rsautl->opts |= RSAUTL_DECRYPT;
	return (0);
}

static int 	set_rsault_pubin(char **argv, t_rsautl *rsautl, int *index)
{
	(void)argv;
	(void)index;
	rsautl->opts |= RSAUTL_PUBIN;
	return (0);
}

const struct s_rsautl_opts  g_rsautl_opts[] = {
	{"-in", set_rsautl_in},
	{"-out", set_rsautl_out},
	{"-inkey", set_inkey_file},
	{"-pubin", set_rsault_pubin},
	{"-hexdump", set_hexdump},
	{"-encrypt", set_encrypt},
	{"-decrypt", set_decrypt},
	{NULL, NULL}
};

int 	rsautl_opts(char **argv, t_rsautl *rsautl)
{
	int i, j;

	i = 1;
	while (argv[++i])
	{
		j = -1;
		while (g_rsautl_opts[++j].opt)
		{
			if (ft_strcmp(g_rsautl_opts[j].opt, argv[i]) == 0)
			{
				if (g_rsautl_opts[j].opt_f(argv, rsautl, &i))
					return (1);
				break ;	
			}
		}
		if (g_rsautl_opts[j].opt == NULL)
		{
			ft_putstr_fd("Unknown option: ", STDERR_FILENO);
			ft_putendl_fd(argv[i], STDERR_FILENO);
			return (1);
		}
	}
	return (0);
}