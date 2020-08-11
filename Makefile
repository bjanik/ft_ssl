NAME = ft_ssl

CC = gcc

FLAGS = -Wall -Wextra -Werror -g

LIBFT_DIR  = libft/

LIBFT_LIB = $(LIBFT_DIR)libft.a
LIBFT_INC = $(LIBFT_DIR)includes/


SRC_PATH = srcs/
OBJ_PATH = obj/
INC_PATH = includes/

SRC_NAME = base64/base64_core.c \
		base64/base64_decode.c \
		base64/base64_encode.c \
		base64/base64_init.c \
		bn/bn_add.c \
		bn/bn_clear.c \
		bn/bn_clone.c \
		bn/bn_copy.c \
		bn/bn_cmp.c \
		bn/bn_div.c \
		bn/bn_init.c \
		bn/bn_mod.c \
		bn/bn_mod_pow.c \
		bn/bn_modinv.c \
		bn/bn_mul.c \
		bn/bn_realloc.c \
		bn/bn_set.c \
		bn/bn_shift.c \
		bn/bn_sub.c \
		bn/bn_utils.c \
		core/commands.c \
		core/ft_ssl.c \
		core/interactive_mode.c \
		core/main.c \
		core/msg.c \
		core/opt.c \
		core/output.c \
		core/padding.c \
		core/update.c \
		core/usage.c \
		core/utils.c \
		des/des.c \
		des/des_bc.c \
		des/des_cbc.c \
		des/des_core.c \
		des/des_decrypt.c \
		des/des_decrypt2.c \
		des/des_ecb.c \
		des/des_encrypt.c \
		des/des_init.c \
		des/des_opts.c \
		des/des_opts2.c \
		des/des_opts3.c \
		des/des_pcbc.c \
		des/gen_key.c \
		des/permutations.c \
		des/reset_des.c \
		des/substitution.c \
		genrsa/genrsa.c\
		genrsa/genrsa_clear.c\
		genrsa/genrsa_init.c\
		genrsa/genrsa_opts.c\
		lexer/delimitate_token.c \
		lexer/init_lexer.c \
		lexer/lexer.c \
		lexer/lst_to_tab.c \
		lexer/realloc_current_token.c \
		lexer/reset_lexer.c \
		md5/md5.c \
		md5/md5_transform.c \
		pbkdf/get_password.c \
		pbkdf/get_salt.c \
		pbkdf/pbkdf.c \
		rsa/flag_check.c \
		rsa/flag_hexdump.c \
		rsa/flag_modulus.c \
		rsa/flag_text.c \
		rsa/get_data.c \
		rsa/lib_bn.c \
		rsa/miller_rabin.c \
		rsa/passphrase.c \
		rsa/pem.c \
		rsa/pem_decode.c \
		rsa/pem_public_key.c \
		rsa/pem_utils.c \
		rsa/print_keys.c \
		rsa/private_key_encryption_decryption.c \
		rsa/rsa.c \
		rsa/rsa_clear.c \
		rsa/rsa_file.c \
		rsa/rsa_init.c \
		rsa/rsa_opts.c \
		rsa/rsa_opts2.c \
		rsa/rsa_opts3.c \
		rsa/rsa_opts_password.c \
		rsautl/rsautl.c \
		rsautl/rsautl_clear.c \
		rsautl/rsautl_decryption.c \
		rsautl/rsautl_encryption.c \
		rsautl/rsautl_init.c\
		rsautl/rsautl_opts.c\
		rsautl/rsautl_opts2.c\
		sha1/sha1.c \
		sha1/sha1_transform.c \
		sha256/sha256.c \
		sha256/sha256_transform.c

OBJ_NAME = $(SRC_NAME:.c=.o)

SRC = $(addprefix $(SRC_PATH),$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH),$(OBJ_NAME))

all : $(NAME)

$(NAME) : $(OBJ)
	@make -C $(LIBFT_DIR)
	@$(CC) $(FLAGS) $(OBJ) $(LIBFT_LIB) -I $(INC_PATH) -I $(LIBFT_INC) -o $(NAME)

$(OBJ) : $(OBJ_PATH)

$(OBJ_PATH) :
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(dir $(OBJ))

$(OBJ_PATH)%.o: $(SRC_PATH)%.c 
	$(CC) $(FLAGS) -MMD -c $< -o $@ -I$(INC_PATH) -I$(LIBFT_INC)

clean :
	/bin/rm -rf $(OBJ_PATH)
	make clean -C $(LIBFT_DIR)

fclean : clean
	make fclean -C $(LIBFT_DIR)
	/bin/rm -f $(NAME)

re : fclean all

-include $(OBJ:.o=.d)

.PHONY: all, clean, fclean, re
