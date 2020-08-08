#include "ft_ssl.h"

int 		set_inform(char **argv, t_rsa *rsa, int *index)
{
	(*index)++;
	rsa->inform = ft_strdup(argv[*index]);
	if (rsa->inform == NULL)
		return (1);
	return (0);
}

int 		set_outform(char **argv, t_rsa *rsa, int *index)
{
	(*index)++;
	rsa->outform = ft_strdup(argv[*index]);
	if (rsa->outform == NULL)
		return (1);
	return (0);
}

int		set_in_file(char **argv, t_rsa *rsa, int *index)
{
	rsa->in = ft_strdup(argv[++(*index)]);
	if (rsa->in == NULL)
		return (1);
	return (0);
}

int		set_out_file(char **argv, t_rsa *rsa, int *index)
{
	rsa->out = ft_strdup(argv[++(*index)]);
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