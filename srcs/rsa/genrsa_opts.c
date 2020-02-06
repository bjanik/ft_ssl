#include "ft_ssl.h"

struct			s_genrsa_opts
{
	char		*opt;
	int			(*opt_func)(char **argv, t_genrsa *genrsa, int *index);
};


static int 	set_genrsa_input(char **argv, t_genrsa *genrsa, int *index)
{
	if ((genrsa->in = argv[++(*index)]) == NULL)
		return (1);
	return (0);
}

static int 	set_genrsa_output(char **argv, t_genrsa *genrsa, int *index)
{
	if ((genrsa->out = argv[++(*index)]) == NULL)
		return (1);
	return (0);
}

static int 	set_random(char **argv, t_genrsa *genrsa, int *index)
{
	if ((genrsa->rand_file = argv[++(*index)]) == NULL)
		return (1);
	return (0);
}

static int 	finalize_genrsa_opts(t_genrsa *genrsa)
{
	if (genrsa->in)
	{
		if ((genrsa->fd[IN] = open(genrsa->in, O_RDONLY)) < 0)
			return (1);
	}
	if (genrsa->out)
	{
		if ((genrsa->fd[OUT] = open(genrsa->out, O_WRONLY)) < 0)
			return (1);	
	}
	return (0);
}

const struct s_genrsa_opts	g_genrsa_opts[] = {
	{"-i", set_genrsa_input},
	{"-o", set_genrsa_output},
	{"-rand", set_random},
	{NULL, NULL}
};

int 	genrsa_opts(char **argv, t_genrsa *genrsa)
{
	int i, j;

	i = 1;
	while (argv[++i])
	{
		j = -1;
		if (ft_str_isdigit(argv[i]))
		{
			genrsa->numbits = ft_atoi(argv[i]);
			break ;
		}
		while (g_genrsa_opts[++j].opt)
		{
			if (ft_strcmp(g_genrsa_opts[j].opt, argv[i]) == 0)
			{
				if (g_genrsa_opts[j].opt_func(argv, genrsa, &i))
					return (1);
				break ;
			}
		}
	}
	return (finalize_genrsa_opts(genrsa));
}