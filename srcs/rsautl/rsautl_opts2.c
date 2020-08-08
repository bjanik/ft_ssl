#include "ft_ssl.h"

int 	set_inkey_file(char **argv, t_rsautl *rsautl, int *index)
{
	(*index)++;
	if (argv[*index] == NULL)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: Missing inkey file\n");
		return (1);
	}
	if ((rsautl->inkey = ft_strdup(argv[*index])) == NULL)
		return (1);
	return (0);
}

int 	set_hexdump(char **argv, t_rsautl *rsautl, int *index)
{
	(void)argv;
	(void)index;
	rsautl->opts |= RSAUTL_HEXDUMP;
	return (0);
}

int 	set_encrypt(char **argv, t_rsautl *rsautl, int *index)
{
	(void)argv;
	(void)index;
	rsautl->opts |= RSAUTL_ENCRYPT;
	return (0);
}

int 	set_decrypt(char **argv, t_rsautl *rsautl, int *index)
{
	(void)argv;
	(void)index;
	rsautl->opts |= RSAUTL_DECRYPT;
	return (0);
}

int 	set_rsault_pubin(char **argv, t_rsautl *rsautl, int *index)
{
	(void)argv;
	(void)index;
	rsautl->opts |= RSAUTL_PUBIN;
	return (0);
}