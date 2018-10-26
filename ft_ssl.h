// HEADER 42
#ifndef FT_SSL_H
# define FT_SSL_H
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>


# define FT_SSL_USAGE "usage: ft_ssl command [command opts] [command args]\n"
# define MD5_USAGE "md5: usage: md5 [-pqr] -s [string] [files ...]\n"
# define MD5_OPTS "pqrs"
# define SHA_OPTS "pqrs"
# define OPT_P 1
# define OPT_Q 2
# define OPT_R 4
# define END_OF_OPT "--"
# define BUF_SIZE 64


typedef struct 		s_ssl_command
{
	const char 		*name;
	unsigned char	*(*cmd)(char *msg, uint32_t msg_len, int fd, uint32_t opts);
	void			(*print_hash)(unsigned char *digest);
	const char		*available_opts;
}					t_ssl_command;

typedef struct 		s_ssl {
	uint32_t 		opts;
	char 			*msg;
	t_ssl_command	*ssl_command;
}					t_ssl;

typedef struct 		s_md5ctx
{
	uint32_t		A;
	uint32_t		B;
	uint32_t		C;
	uint32_t		D;
	uint32_t		len;
	uint64_t		bitlen;
	unsigned char	block[64];
	unsigned char	digest[16];
	int				fd;
}					t_md5ctx;

typedef struct 		s_sha256ctx 
{
	uint32_t		h[8];
	uint32_t		len;
	uint64_t		bitlen;
	unsigned char	block[64];
	unsigned char	digest[32];
	int				fd;
}					t_sha256ctx;		

t_ssl_command		*get_ssl_command(const char *command);
unsigned char 		*md5(char *msg, uint32_t msg_len, int fd, uint32_t opts);
void				md5_transform(t_md5ctx *md5ctx);
void 				md5_update(t_md5ctx *md5ctx, char *msg, uint32_t msg_len);
void				md5_final(t_md5ctx *md5ctx);
void				md5_print(unsigned char *digest);

unsigned char 		*sha256(char *msg, uint32_t msg_len, int fd, uint32_t opts);
void				sha256_transform(t_sha256ctx *sha256ctx);
void				sha256_print(unsigned char *digest);
char				*read_from_stdin(void);

uint32_t			rotleft(uint32_t x, uint32_t n);
uint32_t			rotright(uint32_t x, uint32_t n);

extern t_ssl_command g_commands[3];
#endif