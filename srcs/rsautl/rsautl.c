#include "bn.h"
#include "ft_ssl.h"

static int 				get_private_key_from_inkey(t_rsautl *rsautl,
												   char *data)
{
	unsigned char 	*raw_data;
	uint32_t		raw_data_len;
	int 			ret;
	char			*args[2];

	raw_data_len = 0;
	ret = 0;
	args[0] = rsautl->in;
	raw_data = base64_decode_data(&raw_data_len, ft_strdup(data),
								  ft_strlen(data));
	if (raw_data == NULL)
		return (1);
	if (rsautl->des)
	{
		if ((raw_data = private_key_decryption(rsautl->des,
											   raw_data,
											   &raw_data_len,
											   args)) == NULL)
			return (1);
	}
    ret = parse_decoded_data(&rsautl->rsa_data, raw_data, raw_data_len,
    					  	 rsautl->opts);
    ft_strdel((char**)&raw_data);
    return (ret);
}

static int rsautl_handle_key(t_rsautl *rsautl, char **data, int inkey_fd)
{
	if (rsautl->opts & RSAUTL_PUBIN)
	{
		if (inkey_fd < 0)
			return (ft_dprintf(STDERR_FILENO,
					"ft_ssl: unable to load public key\n"));
		if (!(*data = get_data(inkey_fd, &rsautl->des, PEM_PUBLIC_HEADER,
							   PEM_PUBLIC_FOOTER)))
			return (1);
		if (retrieve_data_from_public_key(&rsautl->rsa_data, *data))
			return (1);
	}
	else
	{
		if (inkey_fd < 0)
			return (ft_dprintf(STDERR_FILENO,
					"ft_ssl: unable to load private key\n"));
		if (!(*data = get_data(inkey_fd, &rsautl->des, PEM_PRIVATE_HEADER,
							   PEM_PRIVATE_FOOTER)))
			return (1);
		if (get_private_key_from_inkey(rsautl, *data))
			return (1);
	}
	return (0);
}

int 				rsautl_command_run(t_rsautl *rsautl)
{
	char	*data;
	int 	inkey_fd;
	int 	ret;

	data = NULL;
	if ((rsautl->opts & RSAUTL_PUBIN) && (rsautl->opts & RSAUTL_DECRYPT))
		return (ft_dprintf(STDERR_FILENO,
				"ft_ssl: private key is required for this operation\n"));
	inkey_fd = open(rsautl->inkey, O_RDONLY);
	if (rsautl->in)
		rsautl->fd[IN] = open(rsautl->in, O_RDONLY);
	ret = rsautl_handle_key(rsautl, &data, inkey_fd);
	ft_strdel(&data);
	close(inkey_fd);
	if (ret)
		return (1);
	if (rsautl->out)
		rsautl->fd[OUT] = open(rsautl->out,
							   O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (rsautl->opts & RSAUTL_DECRYPT)
		return (rsa_message_decryption(&rsautl->rsa_data, rsautl->fd));
	else
		return (rsa_message_encryption(&rsautl->rsa_data, rsautl->fd,
									   rsautl->opts));
}