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

void	rsautl_clear(t_rsautl *rsautl)
{
	ft_strdel(&rsautl->in);
	ft_strdel(&rsautl->out);
	ft_strdel(&rsautl->inkey);
	reset_des(rsautl->des);
	bn_clear(&rsautl->rsa_data.modulus);
	bn_clear(&rsautl->rsa_data.private_exp);
	bn_clear(&rsautl->rsa_data.public_exp);
	bn_clear(&rsautl->rsa_data.prime1);
	bn_clear(&rsautl->rsa_data.prime2);
	bn_clear(&rsautl->rsa_data.exponent1);
	bn_clear(&rsautl->rsa_data.exponent2);
	bn_clear(&rsautl->rsa_data.coef);
	ft_memdel((void**)&rsautl);
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
		if ((raw_data = private_key_decryption(rsautl->des, raw_data, &raw_data_len, rsautl->in, NULL)) == NULL)
			return (1);
	}
    ret = parse_decoded_data(&rsautl->rsa_data, raw_data, raw_data_len, rsautl->opts);
    ft_strdel((char**)&raw_data);
    return (ret);
}

int 				rsautl_command_run(t_rsautl *rsautl)
{
	char	*data = NULL;
	int 	inkey_fd;

	if ((rsautl->opts & RSAUTL_PUBIN) && (rsautl->opts & RSAUTL_DECRYPT))
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: private key is required for this operation\n");
		return (1);
	}
	inkey_fd = open(rsautl->inkey, O_RDONLY);
	if (rsautl->in)
		rsautl->fd[IN] = open(rsautl->in, O_RDONLY);
	if (rsautl->opts & RSAUTL_PUBIN)
	{
		if (inkey_fd < 0)
		{
			ft_dprintf(STDERR_FILENO, "ft_ssl: unable to load public key\n");
			return (1);
		}
		if (!(data = get_data(inkey_fd, &rsautl->des, PEM_PUBLIC_HEADER, PEM_PUBLIC_FOOTER)))
			return (1);
		retrieve_data_from_public_key(&rsautl->rsa_data, data);
	}	
	else
	{
		if (inkey_fd < 0)
		{
			ft_dprintf(STDERR_FILENO, "ft_ssl: unable to load private key\n");
			return (1);
		}
		if (!(data = get_data(inkey_fd, &rsautl->des, PEM_PRIVATE_HEADER, PEM_PRIVATE_FOOTER)))
			return (1);
		if (get_private_key_from_inkey(rsautl, data))
			return (1);
	}
	ft_strdel(&data);
	close(inkey_fd);
	if (rsautl->out)
		rsautl->fd[OUT] = open(rsautl->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (rsautl->opts & RSAUTL_DECRYPT)
		return (rsa_message_decryption(&rsautl->rsa_data, rsautl->fd));
	else
		return (rsa_message_encryption(&rsautl->rsa_data, rsautl->fd, rsautl->opts));
}