#include "bn.h"
#include "ft_ssl.h"

static void handle_errors(int ret,
						 uint32_t mlen,
						 uint32_t modlen,
						 unsigned char **message)
{
	if (ret < 0)
	{
		ft_dprintf(STDERR_FILENO,
				   "ft_ssl: error while reading encrypted message\n");
		ft_memdel((void**)message);
	}
	else if (mlen != modlen)
	{
		ft_dprintf(STDERR_FILENO,
				   "ft_ssl: message to decrypt not equal to modulus len\n");
		ft_memdel((void**)message);
	}
}

static unsigned char *get_encrypted_message(const int fd, uint32_t mod_len)
{
	unsigned char	buf[8];
	unsigned char 	*message;
	uint32_t 		mlen;
	int 			ret;

	mlen = 0;
	if ((message = (unsigned char *)malloc(mod_len)) == NULL)
		return (NULL);
	while ((ret = read(fd, buf, 8)) > 0)
	{
		if (mlen + ret > mod_len)
		{
			ft_dprintf(STDERR_FILENO, "ft_ssl: message to encrypt too long\n");
			free(message);
			return (NULL);
		}
		ft_memcpy(message + mlen, buf, ret);
		mlen += ret;
	}
	handle_errors(ret, mlen, mod_len, &message);
	return (message);
}

static int 			check_decryption(unsigned char *decrypted_msg)
{
	unsigned char 	*ptr;
	int 			i;

	i = 0;
	ptr = decrypted_msg;
	if (*ptr++ != 0x0)
		return (-1);
	if (*ptr++ != 0x02)
		return (-1);
	while (*ptr++ != 0x0)
		i++;
	if (i < 8)
		return (-1);
	return (ptr - decrypted_msg);
}

static int init_plain_and_cipher(t_bn *bn[],
								 unsigned char *msg[],
								 uint32_t mod_len)
{
	msg[1] = msg[0];
	if ((bn[0] = get_bn(&msg[1], mod_len)) == NULL)
	{
		free(msg[0]);
		return (1);
	}
	if ((bn[1] = bn_init_size(mod_len * 8)) == NULL)
	{
		free(msg[0]);
		bn_clear(&bn[0]);
		return (1);
	}
	return (0);
}

int 				rsa_message_decryption(t_rsa_data *rsa_data,
										   const int fd[])
{
	unsigned char	*msg[2];
	unsigned char	*decrypted_msg;
	t_bn 			*bn[2];
	uint32_t		mod_len;
	int 			ret;

	mod_len = bn_len(rsa_data->modulus);
	if ((msg[0] = get_encrypted_message(fd[IN], mod_len)) == NULL)
		return (1);
	if (init_plain_and_cipher(bn, msg, mod_len))
		return (1);
	bn_mod_pow(bn[1], bn[0], rsa_data->private_exp, rsa_data->modulus);
	decrypted_msg = (unsigned char*)malloc(mod_len + 1);
	if (decrypted_msg == NULL)
		return (1);
	decrypted_msg[0] = 0;
	write_bn_to_data(bn[1], decrypted_msg + 1);
	ret = check_decryption(decrypted_msg);
	(ret == -1) ? ft_putendl_fd("ft_ssl: decryption error", STDERR_FILENO) :
				  write(fd[OUT], decrypted_msg + ret, mod_len - ret);
	ret = 0;
	free(msg[0]);
	free(decrypted_msg);
	bn_clears(2, &bn[0], &bn[1]);
	return (ret);
}
