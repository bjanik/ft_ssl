#include "ft_ssl.h"
// #include "fcntl.h"
// #include "unistd.h"

struct			s_rsa_opts
{
	char		*opt;
	int			(*opt_f)(char **argv, t_rsa *rsa, int *index);
};

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

const struct s_rsa_opts  g_rsa_opts[] = {
	{"-inform", set_inform},
	{"-outform", set_outform},
	{"-in", set_in_file},
	{"-out", set_out_file},
	{"-passin", set_password_rsa},
	{"-passout", set_password_rsa},
	{"-des", set_des_encryption},
	{"-text", set_text},
	{"-noout", set_noout},
	{"-modulus", set_modulus},
	{"-check", set_check},
	{"-pubin", set_pubin},
	{"-pubout", set_pubout},
	{NULL, NULL}
};

int 	rsa_opts(char **argv, t_rsa *rsa)
{
	int i;
	int j;

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
			ft_dprintf(STDERR_FILENO, "Unknown option: %s\n", argv[i]);
			return (1);
		}
	}
	return (0);
}