//HEADER 42
# include "ft_ssl.h"

void			md5_init(t_md5ctx *md5ctx, int fd)
{
	md5ctx->len = 0;
	md5ctx->bitlen = 0;
	md5ctx->A = 0x67452301;
	md5ctx->B = 0xefcdab89;
	md5ctx->C = 0x98badcfe;
	md5ctx->D = 0x10325476;
	memset(md5ctx->block, 0x0, 64);
	memset(md5ctx->digest, 0x0, 16);
	md5ctx->fd = fd;
}

void 			md5_update(t_md5ctx *md5ctx, char *msg, uint32_t msg_len)
{
	uint32_t 	i;
	int 		ret;

	i = 0;
	if (msg)
		while (i < msg_len)
		{
			md5ctx->block[md5ctx->len++] = msg[i++];
			if (md5ctx->len == 64)
			{
				md5_transform(md5ctx);
				md5ctx->len = 0;
				md5ctx->bitlen += 512;
			}
		}
	else
		while ((ret = read(md5ctx->fd, md5ctx->block, 64)) > 0)
		{
			if (!md5ctx->fd)
				printf("%s", md5ctx->block);
			if (ret == 64)
			{
				md5_transform(md5ctx);
				md5ctx->bitlen += 512;
			}	
			md5ctx->len = ret;
		}
}

void			md5_final(t_md5ctx *md5ctx)
{
	int		i;

	md5ctx->block[md5ctx->len] = 0x80;
	if (md5ctx->len < 56)
		memset(md5ctx->block + md5ctx->len + 1, 0x0, 56 - md5ctx->len);
	else
	{
		memset(md5ctx->block + md5ctx->len + 1, 0x0, 64 - md5ctx->len);
		md5_transform(md5ctx);
		memset(md5ctx->block, 0x0, 56);
	}
	i = -1;
	md5ctx->bitlen += md5ctx->len * 8;
	while (++i < 8)
		md5ctx->block[56 + i] = md5ctx->bitlen >> i * 8;
	md5_transform(md5ctx);
	i = -1;
	while (++i < 4)
	{
		md5ctx->digest[i] = (md5ctx->A >> (i * 8)) & 0x000000ff;
		md5ctx->digest[i + 4]  = (md5ctx->B >> (i * 8)) & 0x000000ff;
		md5ctx->digest[i + 8]  = (md5ctx->C >> (i * 8)) & 0x000000ff;
		md5ctx->digest[i + 12] = (md5ctx->D >> (i * 8)) & 0x000000ff;
	}
}

void	md5_print(unsigned char *digest)
{
	int 	i;

	i = 0;
	if (digest)
	{
		while (i < 16)
			printf("%02x", digest[i++]);
		putchar('\n');
	}
}

unsigned char	*md5(char *msg, uint32_t msg_len, int fd, uint32_t opts)
{
	t_md5ctx 		md5ctx;
	unsigned char	*digest = malloc(16);

	md5_init(&md5ctx, fd);
	md5_update(&md5ctx, msg, msg_len);
	md5_final(&md5ctx);
	memcpy(digest, md5ctx.digest, 16);
	// md5_print(md5ctx);
	return (digest);
}
