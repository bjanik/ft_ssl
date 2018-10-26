//HEADER 42
# include "ft_ssl.h"

void			sha256_init(t_sha256ctx *ctx, int fd)
{
	ctx->h[0] = 0x6a09e667;
	ctx->h[1] = 0xbb67ae85;
	ctx->h[2] = 0x3c6ef372;
	ctx->h[3] = 0xa54ff53a;
	ctx->h[4] = 0x510e527f;
	ctx->h[5] = 0x9b05688c;
	ctx->h[6] = 0x1f83d9ab;
	ctx->h[7] = 0x5be0cd19;
	ctx->len = 0;
	ctx->bitlen = 0;
	memset(ctx->block, 0x0, 64);
	memset(ctx->digest, 0x0, 32);
	ctx->fd = fd;
}

void 			sha256_update(t_sha256ctx *sha256ctx, char *msg, uint32_t msg_len)
{
	uint32_t 	i;
	int 		ret;

	i = 0;
	if (msg)
		while (i < msg_len)
		{
			sha256ctx->block[sha256ctx->len++] = msg[i++];
			if (sha256ctx->len == 64)
			{
				sha256_transform(sha256ctx);
				sha256ctx->len = 0;
				sha256ctx->bitlen += 512;
			}
		}
	else
		while ((ret = read(sha256ctx->fd, sha256ctx->block, 64)) > 0)
		{
			if (!sha256ctx->fd)
				printf("%s", sha256ctx->block);
			if (ret == 64)
			{
				sha256_transform(sha256ctx);
				sha256ctx->bitlen += 512;
			}	
			sha256ctx->len = ret;
		}
}

void			sha256_final(t_sha256ctx *ctx)
{
	int		i;

	ctx->block[ctx->len] = 0x80;
	if (ctx->len < 56)
		memset(ctx->block + ctx->len + 1, 0x0, 56 - ctx->len);
	else
	{
		memset(ctx->block + ctx->len + 1, 0x0, 64 - ctx->len);
		sha256_transform(ctx);
		memset(ctx->block, 0x0, 56);
	}
	i = -1;
	ctx->bitlen += ctx->len * 8;
	while (++i < 8)
		ctx->block[63 - i] = ctx->bitlen >> i * 8;
	sha256_transform(ctx);
	i = -1;
	while (++i < 4)
	{
		ctx->digest[i] = (ctx->h[0] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 4]  = (ctx->h[1] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 8]  = (ctx->h[2] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 12] = (ctx->h[3] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 16] = (ctx->h[4] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 20] = (ctx->h[5] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 24] = (ctx->h[6] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 28] = (ctx->h[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

void			sha256_print(unsigned char *digest)
{
	int 	i;

	i = 0;
	if (digest)
	{
		while (i < 32)
			printf("%02x", digest[i++]);
		putchar('\n');
	}
}

unsigned char *sha256(char *msg, uint32_t msg_len, int fd, uint32_t opts)
{
	t_sha256ctx	sha256ctx;
	unsigned char 	*digest = malloc(32);

	sha256_init(&sha256ctx, fd);
	sha256_update(&sha256ctx, msg, msg_len);
	sha256_final(&sha256ctx);
	memcpy(digest, sha256ctx.digest, 32);
	sha256_print(digest);
	return (NULL);
}