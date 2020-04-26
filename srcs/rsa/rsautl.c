#include "bn.h"
#include "ft_ssl.h"

t_rsautl	*rsautl_init(void)
{
	t_rsautl 	*rsautl;

	rsautl = (t_rsautl*)malloc(sizeof(t_rsautl));
	if (rsautl == NULL)
		return (NULL);
	rsautl->in = NULL;
	rsautl->out = NULL;
	rsautl->inkey = NULL;
	rsautl->fd[IN] = STDIN_FILENO;
	rsautl->fd[OUT] = STDOUT_FILENO;
	rsautl->opts = 0;
	rsautl->des = NULL;
	return (rsautl);
}

static int 				get_private_key_from_inkey(t_rsautl *rsautl, char *data)
{
	unsigned char 	*raw_data;
	uint32_t		raw_data_len = 0;
	int 			ret = 0;

	raw_data = base64_decode_data(&raw_data_len, ft_strdup(data), ft_strlen(data));
	if (raw_data == NULL)
		return (1);
	if (rsautl->des)
	{
		if ((raw_data = private_key_decryption(rsautl->des, raw_data, &raw_data_len, rsautl->in)) == NULL)
			return (1);
	}
    ret = parse_decoded_data(&rsautl->rsa_data, raw_data, raw_data_len, rsautl->opts);
    return (ret);
}

int 				rsautl_command_run(t_rsautl *rsautl)
{
	char	*data = NULL;
	int 	inkey_fd;

	if ((rsautl->opts & RSAUTL_PUBIN) && (rsautl->opts & RSAUTL_DECRYPT))
	{
		ft_putendl_fd("ft_ssl: private key is required for this operation", STDERR_FILENO);
		return (1);
	}
	inkey_fd = open(rsautl->inkey, O_RDONLY);
	if (rsautl->in)
		rsautl->fd[IN] = open(rsautl->in, O_RDONLY);
	if (rsautl->opts & RSAUTL_PUBIN)
	{
		if (inkey_fd < 0)
		{
			ft_putendl_fd("ft_ssl: unable to load public key", STDERR_FILENO);
			return (1);
		}
		data = get_data(inkey_fd, &rsautl->des, PEM_PUBLIC_HEADER, PEM_PUBLIC_FOOTER);
		retrieve_data_from_public_key(&rsautl->rsa_data, data);
	}	
	else // decryption with private key
	{
		if (inkey_fd < 0)
		{
			ft_putendl_fd("ft_ssl: unable to load private key", STDERR_FILENO);
			return (1);
		}
		if (!(data = get_data(inkey_fd, &rsautl->des, PEM_PRIVATE_HEADER, PEM_PRIVATE_FOOTER)))
			return (1);
		get_private_key_from_inkey(rsautl, data);
	}
	close(inkey_fd);
	if (rsautl->out)
		rsautl->fd[OUT] = open(rsautl->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (rsautl->opts & RSAUTL_DECRYPT)
		rsa_message_decryption(&rsautl->rsa_data, rsautl->fd);
	else if (rsautl->opts & RSAUTL_ENCRYPT)
		rsa_message_encryption(&rsautl->rsa_data, rsautl->fd);			
	return (0);
}