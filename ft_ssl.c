// HEADER 42
#include "ft_ssl.h"

t_ssl_command	g_commands[3] = {
	{"md5", md5, md5_print, MD5_OPTS},
	{"sha256", sha256, sha256_print, SHA_OPTS},
	{NULL, NULL, NULL, NULL},
};

t_ssl_command	*get_ssl_command(const char *command)
{
	int		i;

	i = -1;
	while (g_commands[++i].name)
	{
		if (!strcmp(g_commands[i].name, command))
			return (&g_commands[i]);
	}
	return (NULL);
}

char	*read_from_stdin(void)
{
	char	*buffer;
	char	*tmp;
	size_t	buf_size;
	size_t	len;

	buffer = (char*)malloc(sizeof(char) * (BUF_SIZE + 1));
	bzero(buffer, BUF_SIZE);
	len = 0;
	buf_size = BUF_SIZE;
	while (read(STDIN_FILENO, buffer + len, 64) > 0)
	{
		len++;
		if (len == buf_size)
		{
			buf_size *= 2;
			tmp = buffer;
			buffer = (char*)malloc(sizeof(char) * (buf_size + 1));
			strcpy(buffer, tmp);
			free(tmp);
		}
	}
	buffer[len] = '\0';
	return (buffer);
}