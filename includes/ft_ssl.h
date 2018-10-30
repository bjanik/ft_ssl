/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:18:51 by bjanik            #+#    #+#             */
/*   Updated: 2018/10/29 11:22:03 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>

# include "libft.h"

# define FT_SSL_USAGE "usage: ft_ssl command [command opts] [command args]\n"
# define MD5_USAGE "md5: usage: md5 [-pqr] -s [string] [files ...]\n"
# define SHA_USAGE "sha: usage: md5 [-pqr] -s [string] [files ...]\n"
# define MD5_OPTS "pqrs"
# define SHA_OPTS "pqrs"
# define OPT_P 1
# define OPT_Q 2
# define OPT_R 4
# define END_OF_OPT "--"
# define BUF_SIZE 64


typedef struct 		s_msg
{
	char			*msg;
	uint32_t		msg_len;
	char			*input_file;
	int 			fd;
}					t_msg;

typedef struct		s_ssl_command
{
	const char		*name;
	void			(*cmd_func)(t_msg *msg, uint32_t opts);
	const char		*available_opts;
	uint32_t		opts;
	t_msg			*msg;
}					t_ssl_command;

typedef struct		s_md5ctx
{
	uint32_t		h[4];
	uint32_t		len;
	uint64_t		bitlen;
	unsigned char	block[64];
	unsigned char	digest[16];
	int				fd;
}					t_md5ctx;

typedef struct		s_sha256ctx
{
	uint32_t		h[8];
	uint32_t		len;
	uint64_t		bitlen;
	unsigned char	block[64];
	unsigned char	digest[32];
	int				fd;
}					t_sha256ctx;

union				
{
	t_sha256ctx		*sha256ctx;
	t_md5ctx		*md5ctx;
}					u_ctx;

t_ssl_command		*get_ssl_command(const char *command);
void				md5(t_msg *msg, uint32_t opts);
void				md5_transform(t_md5ctx *md5ctx);

void				sha256(t_msg *msg, uint32_t opts);
void				sha256_transform(t_sha256ctx *sha256ctx);

int 				init_msg(t_msg *msg, char *message, char *input_file);
void				reset_msg(t_msg *msg);

void				print_hash(unsigned char digest[], uint32_t digest_len);

void				usage(void);
void				command_usage(char *command);

uint32_t			rotleft(uint32_t x, uint32_t n);
uint32_t			rotright(uint32_t x, uint32_t n);

extern t_ssl_command	g_commands[3];
#endif
