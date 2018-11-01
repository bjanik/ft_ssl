/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:18:51 by bjanik            #+#    #+#             */
/*   Updated: 2018/11/01 16:12:47 by bjanik           ###   ########.fr       */
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
# define HASH_CMD_USAGE "[-pqr] -s [string] [files ...]\n"
# define MD5_OPTS "pqrs"
# define SHA_OPTS "pqrs"
# define OPT_P 1
# define OPT_Q 2
# define OPT_R 4
# define END_OF_OPT "--"
# define BUF_SIZE 1024
# define BLOCK_SIZE 64

# define MD5_DIGEST_LEN	16
# define SHA1_DIGEST_LEN 20
# define SHA256_DIGEST_LEN 32
# define MAX_CMD_NAME_LEN 6

# define NB_COMMANDS 3

typedef struct			s_msg
{
	char				*msg;
	uint32_t			msg_len;
	char				*input_file;
	int					fd;
	unsigned char		buf[BUF_SIZE + 1];
}						t_msg;

typedef struct			s_ssl_command
{
	const char			*name;
	void				(*cmd_func)(t_msg *msg, uint32_t opts);
	const char			*available_opts;
	uint32_t			opts;
	t_msg				*msg;
}						t_ssl_command;

typedef struct			s_ctx
{
	uint32_t			h[8];
	uint32_t			len;
	uint64_t			bitlen;
	unsigned char		block[BLOCK_SIZE];
	unsigned char		digest[SHA256_DIGEST_LEN];
	uint8_t 			digest_len;
	int					fd;
	void				(*transform)(struct s_ctx *ctx);
	char				cmd_name[MAX_CMD_NAME_LEN + 1];
}						t_ctx;

t_ssl_command			*get_ssl_command(const char *command);

int						update(t_ctx *ctx, t_msg *msg, uint32_t opts);
void					pad_message(t_ctx *ctx);
void					output_digest(t_msg *msg, t_ctx ctx, uint32_t opts);
void					md5(t_msg *msg, uint32_t opts);
void					md5_transform(t_ctx *ctx);

void					sha256(t_msg *msg, uint32_t opts);
void					sha256_transform(t_ctx *ctx);

void					sha1(t_msg *msg, uint32_t opts);
void					sha1_transform(t_ctx *ctx);

int						init_msg(t_msg *msg, char *message, char *input_file);
void					reset_msg(t_msg *msg);

void					print_hash(unsigned char digest[], uint8_t digest_len);

int						usage(void);
int						command_usage(char *command);

uint32_t				rotleft(uint32_t x, uint32_t n);
uint32_t				rotright(uint32_t x, uint32_t n);

extern t_ssl_command	g_commands[NB_COMMANDS + 1];
#endif
