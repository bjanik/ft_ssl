#include "ft_ssl.h"

static int 	rsa_pass_env(char *var_name, char **passwd)
{
	if ((*passwd = ft_strdup(getenv(var_name))) == NULL)
		return (1);
	return (0);
}

static int 	rsa_pass_pass(char *password, char **passwd)
{
	if ((*passwd = ft_strdup(password)) == NULL)
		return (1);
	return (0);
}

static int 	rsa_pass_file(char *filename, char **passwd)
{
	int	fd;

	if ((fd = open(filename, O_RDONLY)) < 0)
		return (1);
	if (get_next_line(fd, passwd) < 0)
		return (1);
	close(fd);
	return (0);
}

static int 	rsa_pass_stdin(char **passwd)
{
	if ((*passwd = ft_strnew(64)) == NULL)
		return (1);
	if (read(STDIN_FILENO, passwd, 64) < 0)
		return (1);
	if (ft_strlen(*passwd) > 0)
		*passwd[ft_strlen(*passwd) - 1] = '\0';
	return (0);
}

int 		set_password_rsa(char **argv, t_rsa *rsa, int *index)
{
	char	**split;
	int 	ret;
	char 	**str;

	split = NULL;
	if (ft_strcmp(argv[(*index)], "-passout") == 0)
		str = &rsa->passout;
	else
		str = &rsa->passin;
	if (argv[(*index) + 1] == NULL)
		ret = (ft_dprintf(STDERR_FILENO, "ft_ssl: Missing password value\n"));
	else if (ft_strcmp(argv[++(*index)], "stdin") == 0)
		ret = rsa_pass_stdin(str);
	else if ((split = ft_strsplit(argv[*index], ':')) == NULL || !split[1])
		ret = 1;
	else if (ft_strcmp(split[0], "pass") == 0)
		ret = rsa_pass_pass(split[1], str);
	else if (ft_strcmp(split[0], "env") == 0)
		ret = rsa_pass_env(split[1], str);
	else if (ft_strcmp(split[0], "file") == 0)
		ret = rsa_pass_file(split[1], str);
	else
		ret = 1;
	ft_free_string_tab(&split);
	return (ret);
}