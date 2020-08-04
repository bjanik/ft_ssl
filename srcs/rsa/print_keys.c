#include "ft_ssl.h"

void 		print_encryption_header(t_rsa *rsa, const int fd)
{
	char	*iv;

	if ((rsa->opts & RSA_DES) && (rsa->opts & RSA_PUBOUT) == 0)
	{
		ft_dprintf(fd, "%s\n%s: DES-CBC,", PROC_TYPE, DEK_INFO);
		iv = ft_itoa_base_llu(convert_input_to_block(rsa->des->salt),
							  "0123456789ABCDEF");
		ft_dprintf(fd, "%s\n\n", iv);
		ft_strdel(&iv);
	}
}

void			print_rsa_key(t_rsa *rsa,
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
	ft_dprintf(fd, "%s\n", public ? PEM_PUBLIC_HEADER : PEM_PRIVATE_HEADER);
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
	ft_dprintf(fd, "%s\n", public ? PEM_PUBLIC_FOOTER : PEM_PRIVATE_FOOTER);
}