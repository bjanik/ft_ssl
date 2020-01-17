NAME = ft_ssl

CC = gcc

FLAGS = -Wall -Wextra -Werror

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
		des/des_ecb.c \
		des/des_init.c \
		des/des_opts.c \
		des/des_opts2.c \
		des/des_opts3.c \
		des/des_pcbc.c \
		des/gen_key.c \
		des/permutations.c \
		des/reset_des.c \
		des/substitution.c \
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
		rsa/rsa_opts.c\
		rsa/genrsa_opts.c\
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
	@$(CC) $(FLAGS) $(OBJ) $(LIBFT_LIB) -I $(INC_PATH) -I $(LIBFT_INC) -o $(NAME) -DGMP

$(OBJ) : $(OBJ_PATH)

$(OBJ_PATH) :
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(dir $(OBJ))

$(OBJ_PATH)%.o: $(SRC_PATH)%.c 
	$(CC) $(FLAGS) -c $< -o $@ -I$(INC_PATH) -I$(LIBFT_INC)

clean :
	/bin/rm -rf $(OBJ_PATH)
	make clean -C $(LIBFT_DIR)

fclean : clean
	make fclean -C $(LIBFT_DIR)
	/bin/rm -f $(NAME)

re : fclean all

.PHONY: all, clean, fclean, re
