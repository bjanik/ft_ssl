#include "bn.h"
#include "ft_ssl.h"

static int 			fill_message_random_bytes(unsigned char *message, int nb, int *offset)
{
	int 			fd;

	if ((fd = open("/dev/random", O_RDONLY)) < 1)
		return (1);
	while (nb)
	{
		message[*offset] = 0;
		while (message[*offset] == 0)
		{
			if (read(fd, &message[*offset], 1) < 0)
				return (1);
		}
		(*offset)++;
		nb--;
	}
	return (0);
}

static unsigned char *get_raw_message(const int fd, uint32_t mod_len, uint32_t *mlen)
{
	unsigned char	buf[8];
	unsigned char 	*raw_message;
	int 			ret;

	*mlen = 0;
	if ((raw_message = (unsigned char *)malloc(mod_len)) == NULL)
		return (NULL);
	while ((ret = read(fd, buf, 8)) > 0)
	{
		if (*mlen + ret > mod_len - 11)
		{
			ft_dprintf(STDERR_FILENO, "ft_ssl: message to encrypt too long\n");
			free(raw_message);
			return (NULL);
		}
		ft_memcpy(raw_message + *mlen, buf, ret);
		*mlen += ret;
	}
	if (ret < 0 || *mlen == 0)
	{
		ft_dprintf(STDERR_FILENO, "ft_ssl: error reading data\n");
		ft_memdel((void**)&raw_message);
	}
	return (raw_message);
}

static unsigned char *get_encoded_message(const int fd, uint32_t mod_len)
{
	unsigned char 	*message, *raw_message;
	int 			offset = 0;
	uint32_t 		mlen = 0;

	if ((message = (unsigned char *)malloc(mod_len)) == NULL)
		return (NULL);
	message[offset++] = 0x0;
	message[offset++] = 0x2;
	if ((raw_message = get_raw_message(fd, mod_len, &mlen)) == NULL)
	{
		ft_memdel((void**)&message);
		return (NULL);
	}
	if (fill_message_random_bytes(message, mod_len - mlen - 3, &offset))
	{
		ft_memdel((void**)&message);
		ft_memdel((void**)&raw_message);
		return (NULL);
	}
	message[offset++] = 0x0;
	ft_memcpy(message + offset, raw_message, mlen);
	ft_memdel((void**)&raw_message);
	return (message);
}

int 	rsa_message_encryption(t_rsa_data *rsa_data, const int fd[], const int opts)
{
	uint32_t		mod_len;
	unsigned char	*plain, *ptr;
	t_bn			*rsa_cipher;
	t_bn			*rsa_message;

	mod_len = bn_len(rsa_data->modulus);
	if ((plain = get_encoded_message(fd[IN], mod_len)) == NULL)
		return (1);
	ptr = plain;
	rsa_message = get_bn(&ptr, mod_len);
	if ((rsa_cipher = bn_init_size(mod_len * 8)) == NULL)
		return (1);
	bn_mod_pow(rsa_cipher, rsa_message, rsa_data->public_exp, rsa_data->modulus);
	ft_memset(plain, 0x0, mod_len);
	write_bn_to_data(rsa_cipher, plain + mod_len - bn_len(rsa_cipher));
	if ((opts & RSAUTL_HEXDUMP))
		flag_hexdump(fd[OUT], plain, mod_len);
	else
		write(fd[OUT], plain, mod_len);
	ft_strdel((char**)&plain);
	bn_clear(&rsa_cipher);
	bn_clear(&rsa_message);
	return (0);
}
