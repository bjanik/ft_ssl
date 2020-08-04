#include "bn.h"
#include "ft_ssl.h"

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
	if (ret < 0)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: error while reading encrypted message\n");
		ft_memdel((void**)&message);
	}
	else if (mlen != mod_len)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: message to decrypt not equal to modulus len\n");
		ft_memdel((void**)&message);
	}
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

int 				rsa_message_decryption(t_rsa_data *rsa_data, const int fd[])
{
	unsigned char	*msg, *ptr, *decrypted_msg;
	t_bn 			*cipher, *plain;
	uint32_t		mod_len;
	int 			ret;

	mod_len = bn_len(rsa_data->modulus);
	if ((msg = get_encrypted_message(fd[IN], mod_len)) == NULL)
		return (1);
	ptr = msg;
	if ((cipher = get_bn(&ptr, mod_len)) == NULL)
	{
		free(msg);
		return (1);
	}
	if ((plain = bn_init_size(mod_len * 8)) == NULL)
	{
		free(msg);
		bn_clear(&cipher);
		return (1);
	}
	bn_mod_pow(plain, cipher, rsa_data->private_exp, rsa_data->modulus);
	decrypted_msg = (unsigned char*)malloc(mod_len + 1);
	if (decrypted_msg == NULL)
		return (1);
	decrypted_msg[0] = 0;
	write_bn_to_data(plain, decrypted_msg + 1);
	ret = check_decryption(decrypted_msg);
	if (ret == -1)
		ft_putendl_fd("ft_ssl: decryption error", STDERR_FILENO);
	else
		write(fd[OUT], decrypted_msg + ret, mod_len - ret);
	ret = 0;
	free(msg);
	free(decrypted_msg);
	bn_clears(2, &cipher, &plain);
	return (ret);
}
