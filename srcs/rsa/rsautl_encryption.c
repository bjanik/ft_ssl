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
			ft_putendl_fd("ft_ssl: message to encrypt too long", STDERR_FILENO);
			free(raw_message);
			return (NULL);
		}
		ft_memcpy(raw_message + *mlen, buf, ret);
		*mlen += ret;
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
	return (message);
}

t_bn 	*os2ip(unsigned char *octet_string, uint32_t len)
{
	t_bn	*n;
	t_bn	*res;
	t_bn	*val;

	n = bn_init_size(len * 64);
	res = bn_init_size(len * 64);
	val = bn_init_size(64);
	if (n == NULL || res == NULL || val == NULL)
	{
		bn_clears(3, &n, &res, &val);
		return (NULL);
	}
	for (uint32_t i = 0; i < len; i++)
	{
		bn_add_ui(val, (uint64_t)octet_string[i]);
		power_of_two(res, (uint64_t)(256 * i));
		bn_mul(res, res, val);
		bn_add(n, n, res);
		bn_set_zero(val);
		bn_set_zero(res);
	}
	return (n);
}

unsigned char	*i2osp(t_bn *n, uint32_t len)
{
	unsigned char	*octet_string;
	t_bn			*digit, *mod, *r;
	int 			i;

	octet_string = (unsigned char*)malloc(len * sizeof(unsigned char));
	if (octet_string == NULL)
		return (NULL);
	digit = bn_init_size(64);
	mod = bn_init_size(64);
	r = bn_init_size(64);
	if (digit == NULL || mod == NULL)
	{
		free(octet_string);
		bn_clears(2, &digit, &mod);
		return (NULL);
	}
	i = 0;
	bn_add_ui(mod, 256);
	while (bn_cmp_ui(n, 0))
	{
		bn_mod(digit, n, mod);
		bn_div(n, r, n, mod);
		octet_string[i] = digit->num[0];
		i++;
	}
	return (octet_string);
}

int 	rsa_message_encryption(t_rsa_data *rsa_data, const int fd[])
{
	uint32_t		mod_len;
	unsigned char	*msg;
	t_bn			*rsa_cipher;
	t_bn			*rsa_message;

	mod_len = bn_len(rsa_data->modulus);
	if ((msg = get_encoded_message(fd[IN], mod_len)) == NULL)
		return (1);
	// rsa_message = os2ip(msg, mod_len);
	if ((rsa_cipher = bn_init_size(mod_len * 8)) == NULL)
		return (1);
	// display_stats(rsa_data->public_exp);
	// display_stats(rsa_data->modulus);
	bn_mod_pow(rsa_cipher, msg, rsa_data->public_exp, rsa_data->modulus);
	write_bn_to_data(rsa_cipher, msg);
	// msg = i2osp(rsa_cipher, mod_len);
	// write(1, msg, mod_len);
	write(fd[OUT], msg, mod_len);
	return (0);
}
