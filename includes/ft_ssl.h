/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:18:51 by bjanik            #+#    #+#             */
/*   Updated: 2018/11/05 10:28:24 by bjanik           ###   ########.fr       */
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
# define HASH_CMD_USAGE " [-pqr] -s [string] [files ...]\n"
# define B64_USG "usage: ft_ssl base64 [-d] [-e] [-i in_file] [-o out_file]\n"
# define HASH_CMD_OPTS "pqrs"
# define OPT_P 1
# define OPT_Q 2
# define OPT_R 4
# define OPT_S 8

# define B64 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
# define BASE64_OPTS "edio"
# define B_OPT_D 1
# define B_OPT_E 2
# define B_OPT_I 4
# define B_OPT_O 8

# define DES_ROUNDS 16
# define DES_OPTS "adeikopsv"
# define DES_OPT_A 1
# define DES_OPT_D 2 
// # define DES_OPT_I 8
// # define DES_OPT_O 32

# define END_OF_OPT "--"
# define BUF_SIZE 4096
# define BLOCK_SIZE 64
# define DES_BLOCK_SIZE 8

# define MD5_DIGEST_LEN 16
# define SHA1_DIGEST_LEN 20
# define SHA256_DIGEST_LEN 32
# define MAX_CMD_NAME_LEN 6

# define QUANTUM_SIZE 3
# define IN 0
# define OUT 1

typedef struct			s_msg
{
	char				*str;
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

typedef struct 			s_base64
{
	char				*input_file;
	char				*output_file;
	uint8_t				encoded[4];
	unsigned char		decoded[3];
	void				(*data[2])(struct s_base64 *base, unsigned char b[]);
	unsigned char		buffer[BUF_SIZE + 1];
	int 				fd[2];
}						t_base64;

typedef struct			s_ctx
{
	uint32_t			h[8];
	uint32_t			len;
	uint64_t			bitlen;
	unsigned char		block[BLOCK_SIZE];
	unsigned char		digest[SHA256_DIGEST_LEN];
	uint8_t				digest_len;
	int					fd;
	void				(*transform)(struct s_ctx *ctx);
	char				cmd_name[MAX_CMD_NAME_LEN + 1];
}						t_ctx;

typedef struct 			s_des
{
	char				*in;
	char				*out;
	unsigned char		input[DES_BLOCK_SIZE];
	uint64_t			keys[DES_ROUNDS];
	uint64_t			init_vector;
	uint8_t				opts;
	int 				fd[2];
}						t_des;

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

int						parse_opt(t_ssl_command *command,
									char **argv,
									int *index);
int						usage(void);
int						commands_usage(char *command);

uint32_t				rotleft(uint32_t x, uint32_t n);
uint32_t				rotright(uint32_t x, uint32_t n);

// void		encode_data(t_base64 *base, unsigned char b[]);
// void		decode_data(t_base64 *base, unsigned char b[]);
void		encode_data2(t_base64 *base);

/*
** DES
*/

void		des_init(t_des *des);

uint64_t	convert_input_to_block(unsigned char input[]);

uint64_t	initial_permutation(uint64_t block);
uint64_t	expansion_permutation(uint32_t r_block);
uint32_t	pbox_permutation(uint32_t block);
uint64_t	final_permutation(uint32_t left, uint32_t right);

void		get_key_from_str(char *str_key, uint64_t *key);
uint64_t	get_56bits_key(uint64_t key);
void		get_subkeys(uint32_t right_key,
						uint32_t left_key,
						uint64_t sub_keys[]);
uint32_t	s_box_substitutions(uint64_t x_block);







extern t_ssl_command 	g_commands[];
#endif