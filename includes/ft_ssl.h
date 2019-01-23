/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:18:51 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 18:05:55 by bjanik           ###   ########.fr       */
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
# include <pwd.h>

# include "libft.h"
# include "lexer.h"

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

# define DES_ROUNDS 16
# define DES_OPTS "adeikopsv"
# define DES_OPT_D 1
# define DES_OPT_A 2
# define DES_NOPAD 4
# define DES_OPT_V 8
# define DES_OPT_K 16
# define DES_OPT_P 32
# define DES_OPT_CAP_P 64

# define MAX_KEY_LEN 16

# define END_OF_OPT "--"
# define BUF_SIZE 4080
# define BASE64_BUF_SIZE 5440
# define BLOCK_SIZE 64
# define DES_BLOCK_SIZE 8

# define MD5_DIGEST_LEN 16
# define SHA1_DIGEST_LEN 20
# define SHA256_DIGEST_LEN 32
# define MAX_CMD_NAME_LEN 6

# define IN 0
# define OUT 1

typedef struct			s_base64
{
	char				*input_file;
	char				*output_file;
	uint8_t				opts;
	uint8_t				encoded[4];
	unsigned char		decoded[3];
	unsigned char		buffer[BASE64_BUF_SIZE + 1];
	int					fd[2];
}						t_base64;

typedef struct			s_des
{
	char				*input_file;
	char				*output_file;
	char				*name;
	char				*password;
	unsigned char		*salt;
	unsigned char		in[BASE64_BUF_SIZE + 1];
	uint64_t			keys[3][DES_ROUNDS];
	unsigned char		hex_keys[64];
	uint64_t			init_vector;
	uint64_t			(*des_mode[2])(uint64_t plain, struct s_des *des);
	uint8_t				opts;
	int					fd[2];
	t_base64			*base64;
}						t_des;

typedef struct			s_des_opts
{
	char				*opt;
	int					(*opt_f)(char **argv, t_des *des, int *index);
}						t_des_opts;

typedef struct			s_msg
{
	unsigned char		*str;
	uint32_t			msg_len;
	char				*input_file;
	int					fd;
	unsigned char		buf[BUF_SIZE + 1];
}						t_msg;

typedef struct			s_ssl_command
{
	char				*name;
	void				(*hash_func)(t_msg *msg, uint32_t opts);
	const char			*available_opts;
	uint32_t			opts;
	t_msg				*msg;
	t_base64			*base64;
	t_des				*des;
	uint64_t			(*des_mode[2])(uint64_t plain, struct s_des *des);
}						t_ssl_command;

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

t_ssl_command			*get_ssl_command(const char *command);
int						interactive_mode(char **argv);
int						ft_ssl_routine(char **argv);

int						update(t_ctx *ctx, t_msg *msg, uint32_t opts);
void					pad_message(t_ctx *ctx);
void					output_digest(t_msg *msg, t_ctx ctx, uint32_t opts);
void					md5(t_msg *msg, uint32_t opts);
void					md5_init(t_ctx *ctx);
unsigned char			*md5_core(t_ctx	*ctx, t_msg *msg, uint32_t opts);
void					md5_transform(t_ctx *ctx);
void					sha256_init(t_ctx *ctx);
void					sha256(t_msg *msg, uint32_t opts);
unsigned char			*sha256_core(t_ctx *ctx, t_msg *msg, uint32_t opts);
void					sha256_transform(t_ctx *ctx);
void					sha1(t_msg *msg, uint32_t opts);
void					sha1_transform(t_ctx *ctx);
int						init_msg(t_msg *msg,
								unsigned char *message,
								char *input_file);
void					reset_msg(t_msg *msg);
void					print_hash(unsigned char digest[],
								uint8_t digest_len,
								uint8_t up);
int						parse_opt(t_ssl_command *command,
									char **argv,
									int *index);
int						usage(void);
int						commands_usage(char *command);

uint32_t				rotleft(uint32_t x, uint32_t n);
uint32_t				rotright(uint32_t x, uint32_t n);
uint64_t				shift_left(uint32_t key, uint8_t x);

/*
** BASE64
*/

void					init_processing(t_base64 *base);
t_base64				*init_base64(void);
int						base64_core(char **argv, t_base64 *base);
void					base64_encode(unsigned char in[], int ret, int fd);
int						base64_decode(unsigned char in[],
									int ret,
									int fd,
									uint8_t des);
int						decode(unsigned char in[],
								unsigned char out[],
								size_t len);

void					init_b64_fds(t_des *des, t_base64 *base);
int						base64_opts(char **argv, t_base64 *base);

/*
** DES
*/

int 					 des_usage(void);
t_des					*init_des(char *name,
								uint64_t (*des_mode[2])(uint64_t plain,
														t_des *des));
int						des_opts(char **argv, t_des *des);
int						set_password(char **argv, t_des *des, int *index);
int						set_subkeys(char **argv, t_des *des, int *index);
int						set_input_file(char **argv, t_des *des, int *index);
int						set_output_file(char **argv, t_des *des, int *index);
int						set_init_vector(char **argv, t_des *des, int *index);
int						set_nopad(char **argv, t_des *des, int *index);
int						set_decrypt_mode(char **argv, t_des *des, int *index);
int						set_encrypt_mode(char **argv, t_des *des, int *index);
int						set_cap_p(char **argv, t_des *des, int *index);
int						set_base64(char **argv, t_des *des, int *index);
int						set_salt(char **argv, t_des *des, int *index);

int						get_hex_from_str(char *str_key, uint64_t *key);
uint64_t				convert_input_to_block(unsigned char input[]);
uint64_t				des_core(uint64_t block, uint64_t sub_keys[]);
void					cipher_to_string(uint64_t cipher,
										unsigned char output[]);

uint64_t				initial_permutation(uint64_t block);
uint64_t				expansion_permutation(uint32_t r_block);
uint32_t				pbox_permutation(uint32_t block);
uint64_t				final_permutation(uint32_t left, uint32_t right);

int						generate_keys_vector(t_des *des);
int						display_skv(t_des *des);

char					*get_password(int encryption);
int						get_salt(t_des *des);
void					get_key_from_str(char *str_key, uint64_t *key);
uint64_t				get_56bits_key(uint64_t key);
void					get_subkeys(uint32_t right_key,
									uint32_t left_key,
									uint64_t sub_keys[]);
void					swap_keys(uint64_t keys[]);
uint32_t				s_box_substitutions(uint64_t x_block);

int						des_encrypt_message(t_des *des);
int						des_decrypt_message(t_des *des);
void					des_get_cipher(t_des *des,
										int offset,
										unsigned char buf[]);
uint64_t				des_ecb_e_d(uint64_t plain, t_des *des);
uint64_t				des_cbc_e(uint64_t plain, t_des *des);
uint64_t				des_cbc_d(uint64_t plain, t_des *des);
uint64_t				des_cfb_e_d(uint64_t plain, t_des *des);
uint64_t				des_bc_e(uint64_t plain, t_des *des);
uint64_t				des_bc_d(uint64_t plain, t_des *des);
uint64_t				des_pcbc_e(uint64_t plain, t_des *des);
uint64_t				des_pcbc_d(uint64_t plain, t_des *des);

uint64_t				des3_bc_e(uint64_t plain, t_des *des);
uint64_t				des3_bc_d(uint64_t plain, t_des *des);
uint64_t				des3_ecb_e(uint64_t plain, t_des *des);
uint64_t				des3_ecb_d(uint64_t cipher, t_des *des);

uint64_t				des3_cbc_e(uint64_t plain, t_des *des);
uint64_t				des3_cbc_d(uint64_t plain, t_des *des);
uint64_t				des3_pcbc_e(uint64_t plain, t_des *des);
uint64_t				des3_pcbc_d(uint64_t plain, t_des *des);
char					**lst_to_tab(t_list *tokens, int count);

extern t_ssl_command	g_commands[];
#endif
