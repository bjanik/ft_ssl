#include "ft_ssl.h"

static int 			init_rsa_data_decryption(t_des **des, char *line)
{
	char	**tab;
	char	**algo_iv;
	int 	ret;

	ret = 0;
	if ((tab = ft_strsplit(line, ':')) == NULL)
		return (1);
	if ((algo_iv = ft_strsplit(tab[1], ',')) == NULL)
	{
		ft_free_string_tab(&tab);
		return (1);
	}
	if (ft_strcmp(algo_iv[0], " DES-CBC") == 0)
		*des = init_des("des-ecb", g_commands[6].des_mode);
	else
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: encryption algorithm not known\n");
		ret = 1;
	}
	if (ret == 0 && get_hex_from_str(algo_iv[1], &(*des)->init_vector))
		ret = 1;
	ft_free_string_tab(&tab);
	ft_free_string_tab(&algo_iv);
	return (ret);
}

static char 	*get_data_final(int ret, char *data)
{
	if (ret < 0)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: read error\n");
		ft_strdel(&data);
		return (NULL);
	}
	return (data);
}


char			*get_data(const int fd, 
						  t_des **des,
						  const char *header,
						  const char *footer)
{
	char		*line;
	char		*data;
	int 		val[3];

	line = NULL;
	data = ft_strnew(64);
	ft_memset((void*)val, 0, sizeof(val));
	while ((val[0] = get_next_line(fd, &line)) > 0)
	{
		if (ft_strcmp(line, header) == 0)
		{
			val[2] = 1;
			continue;
		}
		if (ft_strcmp(line, footer) == 0)
			break ;
		else if (ft_strcmp(line, PROC_TYPE) == 0)
			val[1] = 1;
		else if (val[1] == 1 && ft_strncmp(line, DEK_INFO, 8) == 0)
			init_rsa_data_decryption(des, line);
		else if (data && val[2] == 1)
			if ((data = ft_strjoin_free(data, line, 1)) == NULL)
				break;
		ft_strdel(&line);
	}
	ft_strdel(&line);
	return (get_data_final(val[0], data));
}