#include "bn.h"
#include "ft_ssl.h"

t_rsa				*rsa_init(void)
{
	t_rsa	*rsa;

	if ((rsa = (t_rsa*)malloc(sizeof(t_rsa))) == NULL)
		return (NULL);
	rsa->inform = NULL;
	rsa->outform = NULL;
	rsa->in = NULL;
	rsa->out = NULL;
	rsa->fd[IN] = STDIN_FILENO;
	rsa->fd[OUT] = STDOUT_FILENO;
	rsa->passin = NULL;
	rsa->passout = NULL;
	rsa->opts = 0;
	rsa->des = NULL;
	return (rsa);
}

void				rsa_clear(t_rsa *rsa)
{
	if (rsa)
	{
		ft_strdel(&rsa->inform);
		ft_strdel(&rsa->outform);
		ft_strdel(&rsa->passin);
		ft_strdel(&rsa->passout);
		reset_des(rsa->des);
	}
	ft_memdel((void**)&rsa);
}

static int 			init_rsa_data_decryption(t_des **des, char *line)
{
	char	**tab;
	char	**algo_iv;

	if ((tab = ft_strsplit(line, ':')) == NULL)
		return (1);
	if ((algo_iv = ft_strsplit(tab[1], ',')) == NULL)
	{
		ft_free_string_tab(&tab);
		return (1);	
	}
	if (ft_strcmp(algo_iv[0], " DES-CBC") == 0)
		*des = init_des("des-ecb", g_commands[6].des_mode);
	else if (ft_strcmp(algo_iv[0], " DES-EDE3-CBC") == 0)
		*des = init_des("des3-ecb", g_commands[11].des_mode);
	else
	{
		ft_free_string_tab(&tab);
		ft_free_string_tab(&algo_iv);
		ft_dprintf(STDERR_FILENO, "ft_ssl: encryption algorithm not known\n");
		return (1);
	}
	if (get_hex_from_str(algo_iv[1], &(*des)->init_vector))
	{
		ft_free_string_tab(&tab);
		ft_free_string_tab(&algo_iv);
		return (1);
	}
	ft_free_string_tab(&tab);
	ft_free_string_tab(&algo_iv);
	return (0);
}

char			*get_data(const int fd, t_des **des, const char *header, const char *footer)
{
	char			*line = NULL;
	char			*data = NULL;
	int 			ret = 0, proc_type = 0;

	while ((ret = get_next_line(fd, &line)) > 0)
	{
		if (ft_strcmp(line, header) == 0)
		{
			if ((data = ft_strnew(1)) == NULL)
				return (NULL);
		}
		else if (ft_strcmp(line, footer) == 0)
			break ;
		else if (ft_strcmp(line, PROC_TYPE) == 0)
			proc_type = 1;
		else if (proc_type == 1 && ft_strncmp(line, DEK_INFO, 8) == 0)
			init_rsa_data_decryption(des, line);
		else if (data)
		{
			if ((data = ft_strjoin_free(data, line, 1)) == NULL)
				break;
		}
		ft_strdel(&line);
	}
	ft_strdel(&line);
	if (ret < 0)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: read error\n");
		return (NULL);
	}
	return (data);
}


static void 		print_encryption_header(t_rsa *rsa, const int fd)
{
	char	*iv;

	if (rsa->opts & (RSA_DES | RSA_DES3) && (rsa->opts & RSA_PUBOUT) == 0)
	{
		ft_dprintf(fd, "%s\n%s", PROC_TYPE, DEK_INFO);
		if (ft_strchr(rsa->des->name, '3'))
			ft_dprintf(fd, ": DES-EDE3-CBC,");
		else
			ft_dprintf(fd, ": DES-CBC,");
		iv = ft_itoa_base_llu(convert_input_to_block(rsa->des->salt), "0123456789ABCDEF");
		ft_dprintf(fd, "%s\n\n", iv);
		ft_strdel(&iv);
	}
}

static void			print_rsa_key(t_rsa *rsa,
								  char *data,
								  const int fd,
								  const int rsa_opts)
{
	int 			i;
	int 			len;
	int 			public;

	len = ft_strlen(data);
	i = 0;
	public = 0;
	ft_dprintf(STDERR_FILENO, "writing RSA key\n");
	if (rsa->opts & RSA_PUBIN || rsa->opts & RSA_PUBOUT)
		public = 1;
	ft_putendl_fd(public ? PEM_PUBLIC_HEADER : PEM_PRIVATE_HEADER, fd);
	print_encryption_header(rsa, fd);
	while (len > 0)
	{
		if (len > 64)
			write(fd, data + i, 64);
		else
			write(fd, data + i, len);
		i += 64;
		len -= 64;
		write(fd, "\n", 1);
	}
	ft_putendl_fd(public ? PEM_PUBLIC_FOOTER : PEM_PRIVATE_FOOTER, fd);
}

static char 		*create_public_key(t_rsa *rsa)
{
	unsigned char	*public_data = NULL;
	char			*public_data_encoded;
	uint32_t		public_data_len, total_len;

    public_data_len = get_public_data_len(rsa->rsa_data.modulus, rsa->rsa_data.public_exp);
    total_len = public_data_len + get_byte_number(public_data_len) + (public_data_len >= 0x80) + 1;
    if ((public_data = (unsigned char*)malloc(total_len)) == NULL)
        return (NULL);
    fill_pem_public_data(public_data, public_data_len, rsa->rsa_data.modulus, rsa->rsa_data.public_exp);
    public_data_encoded = base64_encode_data(public_data, public_data_len);
    free(public_data);
    return (public_data_encoded);
}

int 				rsa_private_key_routine(t_rsa *rsa, char **data)
{
	unsigned char 	*raw_data;
	uint32_t		raw_data_len = 0;
	int 			ret = 0;

	raw_data = base64_decode_data(&raw_data_len, ft_strdup(*data), ft_strlen(*data));
	if (raw_data == NULL)
		return (1);
	if (rsa->des)
	{
		if ((raw_data = private_key_decryption(rsa->des, raw_data, &raw_data_len, rsa->in, rsa->passin)) == NULL)
			return (1);
	}
    ret = parse_decoded_data(&rsa->rsa_data, raw_data, raw_data_len, rsa->opts);
    if (rsa->opts & RSA_TEXT)
		flag_text(rsa);
	if (rsa->opts & RSA_MODULUS)
		flag_modulus(rsa->rsa_data.modulus, rsa->fd[OUT]);
	if (rsa->opts & RSA_CHECK)
		ret = flag_check(rsa->rsa_data);
	if (rsa->opts & (RSA_DES | RSA_DES3) && (rsa->opts & RSA_PUBOUT) == 0)
    {
    	reset_des(rsa->des);
 		if (rsa->opts & RSA_DES)
			rsa->des = init_des("des-cbc", g_commands[6].des_mode);
		else
			rsa->des = init_des("des3-cbc", g_commands[11].des_mode);
		if ((raw_data = private_key_encryption(rsa->des, raw_data, &raw_data_len, rsa->in, rsa->passout)) == NULL)
			return (1);
    }
    ft_strdel(data);
    *data = base64_encode_data(raw_data, raw_data_len);
    ft_memdel((void**)&raw_data);
    return (0);
}

static int 			rsa_input_file(t_rsa *rsa)
{
	if (rsa->in)
	{
		if ((rsa->fd[IN] = open(rsa->in, O_RDONLY, 0644)) < 0)
			return (1);
	}
	return (0);
}

static int 			rsa_output_file(t_rsa *rsa)
{
	if (rsa->out)
	{
		if ((rsa->fd[OUT] = open(rsa->out, O_CREAT | O_TRUNC | O_WRONLY, 0644)) < 0)
			return (1);
	}
	return (0);
}

int 				rsa_command_run(t_rsa *rsa)
{
	char			*data = NULL;
	int 			ret = 0;

	rsa_input_file(rsa);
	if ((rsa->opts & RSA_PUBIN) == 0)
	{
		if ((data = get_data(rsa->fd[IN], &rsa->des, PEM_PRIVATE_HEADER, PEM_PRIVATE_FOOTER)) == NULL)
			return (1);
		if (rsa_private_key_routine(rsa, &data) || data == NULL)
		{
			ft_strdel(&data);
			return (1);
		}
	}
	else
	{
		if ((data = get_data(rsa->fd[IN], &rsa->des, PEM_PUBLIC_HEADER, PEM_PUBLIC_FOOTER)) == NULL)
			return (1);
	}
	if (rsa->opts & RSA_PUBIN)
	{
		if (rsa->opts & RSA_CHECK)
		{
			ft_dprintf(STDERR_FILENO, "Only private keys can be checked\n");
			ft_strdel(&data);
			return (1);
		}
		if (retrieve_data_from_public_key(&rsa->rsa_data, data))
			return (1);
		if (rsa->opts & RSA_TEXT)
			flag_text(rsa);
		if (rsa->opts & RSA_MODULUS)
			flag_modulus(rsa->rsa_data.modulus, rsa->fd[OUT]);
	}
	else if (rsa->opts & RSA_PUBOUT)
	{
		if ((data = create_public_key(rsa)) == NULL)
			return (1);
	}
	rsa_output_file(rsa);
	if ((rsa->opts & RSA_NOOUT) == 0)
	 	print_rsa_key(rsa, data, rsa->fd[OUT], rsa->opts);
	ft_strdel(&data);
	return (ret);
}