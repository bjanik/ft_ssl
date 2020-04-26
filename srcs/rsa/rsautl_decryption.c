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
			ft_putendl_fd("ft_ssl: message to encrypt too long", STDERR_FILENO);
			free(message);
			return (NULL);
		}
		ft_memcpy(message + mlen, buf, ret);
		mlen += ret;
	}
	if (ret < 0)
	{
		ft_putendl_fd("ft_ssl: error while reading encrypted message", STDERR_FILENO);
		ft_memdel((void**)&message);
	}
	else if (mlen != mod_len)
	{
		ft_putendl_fd("ft_ssl: message to decrypt not equal to modulus len", STDERR_FILENO);
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
	// write(1, msg, mod_len);
	if ((cipher = get_bn(&ptr, mod_len)) == NULL)
	{
		free(msg);
		return (1);
	}
	// display_stats(cipher);
	// display_bn(cipher);
	if ((plain = bn_init_size(mod_len * 8)) == NULL)
	{
		free(msg);
		bn_clear(&cipher);
		return (1);
	}
	display_stats(rsa_data->public_exp);
	display_stats(rsa_data->modulus);
	bn_mod_pow(plain, cipher, rsa_data->private_exp, rsa_data->modulus);
	decrypted_msg = malloc(mod_len);
	// decrypted_msg = i2osp(plain, mod_len);
	// display_bn(plain);
	// ft_memset(decrypted_msg, 0x0, mod_len);
	write_bn_to_data(plain, decrypted_msg);
	write(1, decrypted_msg, mod_len);
	ret = check_decryption(decrypted_msg);
	if (ret == -1)
		ft_putendl_fd("ft_ssl: decryption error", STDERR_FILENO);
	else
		write(fd[OUT], msg + ret, mod_len - ret);
	return (0);
}
