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

# include "bn.h"
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

# define SINGLE_DES 1
# define TRIPLE_DES 3

# define DES_ROUNDS 16
# define DES_OPTS "adeikopsv"
# define DES_OPT_D 1
# define DES_OPT_A 2
# define DES_NOPAD 4
# define DES_OPT_V 8
# define DES_OPT_K 16
# define DES_OPT_P 32
# define DES_OPT_CAP_P 64

# define GENRSA_DES 1

# define RSA_CHECK 1
# define RSA_DES 2
# define RSA_MODULUS 4
# define RSA_NOOUT 8
# define RSA_TEXT 16
# define RSA_PUBIN 32
# define RSA_PUBOUT 64
# define RSA_PASSIN 128
# define RSA_PASSOUT 256

# define RSAUTL_ENCRYPT 1
# define RSAUTL_DECRYPT 2
# define RSAUTL_HEXDUMP 4
# define RSAUTL_PUBIN 32

# define PEM_PRIVATE_HEADER "-----BEGIN RSA PRIVATE KEY-----"
# define PEM_PRIVATE_FOOTER "-----END RSA PRIVATE KEY-----"
# define PEM_PUBLIC_HEADER "-----BEGIN PUBLIC KEY-----"
# define PEM_PUBLIC_FOOTER "-----END PUBLIC KEY-----"

# define DEK_INFO "DEK-Info"
# define PROC_TYPE "Proc-Type: 4,ENCRYPTED"

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

typedef struct			s_opts
{
	char				*opt;
	int					(*opt_f)(char **argv, void *ptr, int *index);
}						t_opts;

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

typedef struct			s_rsa_data
{
	t_bn				*modulus;
	t_bn				*public_exp;
	t_bn				*private_exp;
	t_bn				*prime1;
	t_bn				*prime2;
	t_bn				*exponent1;
	t_bn				*exponent2;
	t_bn				*coef;
}						t_rsa_data;

typedef struct			s_genrsa
{
	char				*out;
	char				*passout;
	int					fd[2];
	int					numbits;
	t_des				*des;
	t_rsa_data			rsa_data;
	int					opts;
}						t_genrsa;

typedef struct			s_rsa
{
	char				*inform;
	char				*outform;
	char				*in;
	char				*out;
	int					fd[2];
	char				*passin;
	char				*passout;
	uint8_t				opts;
	t_des				*des;
	t_rsa_data			rsa_data;
}						t_rsa;

typedef struct			s_rsautl
{
	char				*in;
	char				*out;
	char				*inkey;
	int					fd[2];
	int					opts;
	t_rsa_data			rsa_data;
	t_des				*des;
}						t_rsautl;

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
	int					(*hash_func)(int opt, unsigned char *message,
										char *input_file);
	uint64_t			(*des_mode[2])(uint64_t plain, struct s_des *des);
	int					(*func)(char **argv, struct s_ssl_command *cmd);
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

t_ssl_command			*get_ssl_command(char **argv);
int						interactive_mode(char **argv);
int						ft_ssl_routine(char **argv);
int						digest_command(char **argv, t_ssl_command *cmd);
int						des_command(char **argv, t_ssl_command *cmd);
int						genrsa_command(char **argv, t_ssl_command *cmd);
int						rsa_command(char **argv, t_ssl_command *cmd);
int						rsautl_command(char **argv, t_ssl_command *cmd);

int						update(t_ctx *ctx, t_msg *msg, uint32_t opts);
void					pad_message(t_ctx *ctx);
void					output_digest(t_msg *msg, t_ctx ctx, uint32_t opts);
int						md5(int opts, unsigned char *message, char *input_file);
void					md5_init(t_ctx *ctx);
unsigned char			*md5_core(t_ctx	*ctx, t_msg *msg, uint32_t opts);
void					md5_transform(t_ctx *ctx);
void					sha256_init(t_ctx *ctx);
int						sha256(int opts, unsigned char *message,
								char *input_file);
unsigned char			*sha256_core(t_ctx *ctx, t_msg *msg, uint32_t opts);
void					sha256_transform(t_ctx *ctx);
int						sha1(int opts, unsigned char *message,
								char *input_file);
void					sha1_transform(t_ctx *ctx);
int						init_msg(t_msg *msg,
									unsigned char *message,
									char *input_file);
void					reset_msg(t_msg *msg);
void					print_hash(unsigned char digest[],
								uint8_t digest_len,
								uint8_t up);
int						hash_files(char **argv, int opts,
									t_ssl_command *command);
int						parse_opt(t_ssl_command *command,
									int *opts,
									char **argv,
									int *index);
int						set_options(t_ssl_command *command, int *opts,
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
int						base64_usage(char *opt);
int						base64_command(char **argv, t_ssl_command *command);
void					base64_encode(unsigned char in[], int ret, int fd);
int						base64_decode(unsigned char in[],
									int ret,
									int fd,
									uint8_t des);
unsigned char			*base64_decode_data(uint32_t *decoded_data_len,
											char *data, uint32_t data_len);
char					*base64_encode_data(unsigned char *data,
											uint32_t data_len);

int						decode(unsigned char in[],
								unsigned char out[],
								size_t len);

void					init_b64_fds(t_des *des, t_base64 *base);
int						base64_opts(char **argv, t_base64 *base);

/*
** DES
*/

int						des_usage(const char *name, char *opt);
t_des					*init_des(char *name,
								uint64_t (*des_mode[2])(uint64_t plain,
														t_des *des));
void					reset_des(t_des *des);
int						des_opts(char **argv, t_des *des);
int						set_password(char **argv, void *ptr, int *index);
int						set_subkeys(char **argv, void *ptr, int *index);
int						set_input_file(char **argv, void *ptr, int *index);
int						set_output_file(char **argv, void *ptr, int *index);
int						set_init_vector(char **argv, void *ptr, int *index);
int						set_nopad(char **argv, void *ptr, int *index);
int						set_decrypt_mode(char **argv, void *ptr, int *index);
int						set_encrypt_mode(char **argv, void *ptr, int *index);
int						set_cap_p(char **argv, void *ptr, int *index);
int						set_base64(char **argv, void *ptr, int *index);
int						set_salt(char **argv, void *ptr, int *index);

int						get_hex_from_str(char *str_key, uint64_t *key);
uint64_t				convert_input_to_block(unsigned char input[]);
uint64_t				des_core(uint64_t block, uint64_t sub_keys[]);
void					cipher_to_string(uint64_t cipher,
										unsigned char output[]);

uint64_t				initial_permutation(uint64_t block);
uint64_t				expansion_permutation(uint32_t r_block);
uint32_t				pbox_permutation(uint32_t block);
uint64_t				final_permutation(uint32_t left, uint32_t right);
unsigned char			*des_encrypt_data(t_des *des, unsigned char *data,
											uint32_t *data_len);
unsigned char			*des_decrypt_data(t_des *des, unsigned char *data,
											uint32_t data_len);

unsigned char			*pbkdf(char *password, unsigned char *salt, int des);
unsigned char			*gen_hash(t_ctx *ctx, t_msg *msg, int des);

int						generate_keys_vector(t_des *des);
void					set_key(t_des *des,
								unsigned char *hash,
								unsigned char keys[][8],
								int nb);
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
int						des_decrypt_message_base64(t_des *des);
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
int						check_errors(unsigned char buf[], int len, int ret);

/*
** RSA flags functions
*/

char					*get_data(const int fd,
									t_des **des,
									const char *header,
									const char *footer);

int						set_inform(char **argv, void *ptr, int *index);
int						set_outform(char **argv, void *ptr, int *index);
int						set_in_file(char **argv, void *ptr, int *index);
int						set_out_file(char **argv, void *ptr, int *index);
int						set_password_rsa(char **argv, void *ptr, int *index);
int						set_password_output(char **argv, void *ptr, int *index);
int						set_des_encryption(char **argv, void *ptr, int *index);
int						set_text(char **argv, void *ptr, int *index);
int						set_noout(char **argv, void *ptr, int *index);
int						set_modulus(char **argv, void *ptr, int *index);
int						set_check(char **argv, void *ptr, int *index);
int						set_pubin(char **argv, void *ptr, int *index);
int						set_pubout(char **argv, void *ptr, int *index);

int						pass_env(char *var_name, char **passwd);
int						pass_pass(char *password, char **passwd);
int						pass_file(char *filename, char **passwd);
int						pass_stdin(char **passwd);

t_genrsa				*genrsa_init(void);
void					genrsa_clear(t_genrsa *rsa);

int						genrsa_opts(char **argv, t_genrsa *rsa);
int						genrsa_usage(char *opt);

int						genrsa_command_run(t_rsa_data *rsa, t_genrsa *genrsa);

int						pem(t_genrsa *genrsa);
char					*pem_encode(unsigned char *data, uint32_t data_len);
int						pem_output(char *data, int fd);
int						parse_decoded_data(t_rsa_data *rsa_data,
											unsigned char *decoded_data,
											uint32_t decoded_data_len,
											int opts);

t_bn					*retrieve_nb_from_data(unsigned char **data,
												unsigned char *start,
												uint32_t len);

uint32_t				get_number_len(unsigned char **ptr);

void					set_len_to_data(uint32_t n, unsigned char *data,
										uint32_t *len);
void					set_bn_to_data(t_bn *n, unsigned char *data,
										uint32_t *len);
void					write_bn_to_data(t_bn *n, unsigned char *data);
uint32_t				get_public_data_len(t_bn *modulus, t_bn *public_exp);
uint32_t				get_pem_data_len(t_rsa_data *rsa_data);
int						retrieve_modulus_and_public(t_rsa_data *rsa_data,
													unsigned char **ptr,
													unsigned char *data,
													uint32_t length);

t_rsa					*rsa_init(void);
void					rsa_clear(t_rsa *rsa);
int						rsa_output_file(t_rsa *rsa);
int						rsa_input_file(t_rsa *rsa);

int						rsa_opts(char **argv, t_rsa *rsa);
int						rsa_usage(char *opt);
int						rsa_command_run(t_rsa *rsa);
unsigned char			*private_key_decryption(t_des *des,
												unsigned char *data,
												uint32_t *data_len,
												char *args[]);
unsigned char			*private_key_encryption(t_des *des,
												unsigned char *data,
												uint32_t *data_len,
												char *args[]);
int						key_from_passphrase(t_des *des,
											const int decryption,
											const char *in,
											const char *pass);

void					flag_modulus(t_bn *n, int fd);
void					flag_text(t_rsa *rsa);
int						flag_check(t_rsa_data rsa_data, int fd);

void					fill_pem_public_data(unsigned char *public_data,
												uint32_t public_data_len,
												t_bn *modulus,
												t_bn *public_exp);
int						retrieve_data_from_public_key(t_rsa_data *rsa_data,
														char *public_data);
void					print_encryption_header(t_des *des, const int fd);
void					print_rsa_key(t_rsa *rsa,
										char *data,
										const int fd);
/*
** RSAUTL
*/

t_rsautl				*rsautl_init(void);
void					rsautl_clear(t_rsautl *rsautl);

int						rsautl_opts(char **argv, t_rsautl *rsautl);
int						set_rsautl_out(char **argv, void *ptr, int *i);
int						set_rsautl_in(char **argv, void *ptr, int *i);
int						set_inkey_file(char **argv, void *ptr, int *i);
int						set_hexdump(char **argv, void *ptr, int *i);
int						set_encrypt(char **argv, void *ptr, int *i);
int						set_decrypt(char **argv, void *ptr, int *i);
int						set_rsault_pubin(char **argv, void *ptr, int *i);
int						rsautl_command_run(t_rsautl *rsautl);
void					flag_hexdump(const int fd,
										const unsigned char *msg,
										const uint32_t len);
int						rsa_message_encryption(t_rsa_data *rsa_data,
												const int fd[],
												const int opts);
int						rsa_message_decryption(t_rsa_data *rsa_data,
												const int fd[],
												const int opts);
void					display_decryption(unsigned char *decrypted_msg,
											const int opts, uint32_t len,
											const int fd);
extern t_ssl_command		g_commands[];
extern const unsigned char	g_padding_patterns[][8];
#endif
