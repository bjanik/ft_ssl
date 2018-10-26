// HEADER 42

# include "ft_ssl.h"

void	output_hash(uint32_t opts, unsigned char *digest, char *name)
{
	if (!opts)
		printf("MD5 (%s) = ", name);
	md5_print(digest);
	if (opts & OPT_R)
		printf(" %s\n", name);
}

void	hash_file(t_ssl ssl, char *filename)
{
	int				fd;
	unsigned char	*digest = malloc(16);

	if ((fd = open(filename, O_RDONLY)) < 0)
		printf("ft_ssl: %s: Cannot open file\n", filename);
	else
	{	
		ssl.ssl_command->cmd(NULL, 0, fd, ssl.opts);
		close(fd);
		output_hash(ssl.opts, digest, filename);
	}
}

int		main_loop(t_ssl *ssl, char **argv)
{
	int				i;
	unsigned char	*digest;

	i = 2;
	digest = NULL;
	while (argv[i] && strcmp(END_OF_OPT, argv[i]) && argv[i][0] == '-')
	{
		if (!strncmp("-s", argv[i], 2))
		{
			if (strlen(argv[i]) > 2)
				ssl->msg = strdup(&argv[i][2]);
			else if (argv[++i])
				ssl->msg = strdup(argv[i]);
			else
			{
				printf("md5: option requires an argument -- s\n");
				printf("%s", MD5_USAGE);
				exit(1);
			}
		}
		else if (!strncmp("-p", argv[i], 2))
		{
			digest = ssl->ssl_command->cmd(NULL, 0, STDIN_FILENO, ssl->opts);
			ssl->opts |= OPT_P;
		}
		else if (!strncmp("-r", argv[i], 2))
			ssl->opts |= OPT_R;
		else if (!strncmp("-q", argv[i], 2))
			ssl->opts |= OPT_Q;
		if (ssl->msg)
			digest = ssl->ssl_command->cmd(ssl->msg, strlen(ssl->msg), -1, ssl->opts);
		i++;
		if (digest)
			output_hash(ssl->opts, digest, ssl->msg);
	}
	return (i);
}

int		main(int argc, char **argv)
{
	t_ssl			ssl = {0, NULL, NULL};
	unsigned char	*digest;
	int				i;

	if (argc == 1)
	{
		write(STDERR_FILENO, FT_SSL_USAGE, strlen(FT_SSL_USAGE));
		return (1);
	}
	if (!(ssl.ssl_command = get_ssl_command(argv[1])))
	{
		write(STDERR_FILENO, "ft_ssl: command not found\n", 26);
		return (1);
	}
	if (argc == 2)
	{
		digest = ssl.ssl_command->cmd(NULL, 0, STDIN_FILENO, ssl.opts);
		ssl.ssl_command->print_hash(digest);
	}
	i = main_loop(&ssl, argv);	
	while (argv[i])
		hash_file(ssl, argv[i++]);
}