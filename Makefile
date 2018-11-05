NAME = ft_ssl

CC = gcc

FLAGS = -Wall -Wextra -Werror

LIBFT  = libft/

LIB = libft/libft.a

HEADERS = includes/

SRC_PATH = srcs/
OBJ_PATH = obj/

SRC_NAME =  core/ft_ssl.c \
		core/main.c \
		core/msg.c \
		core/opt.c \
		core/output.c \
		core/padding.c \
		core/update.c \
		core/usage.c \
		core/utils.c \
		md5/md5.c \
		md5/md5_transform.c \
		sha1/sha1.c \
		sha1/sha1_transform.c \
		sha256/sha256.c \
		sha256/sha256_transform.c

OBJ_NAME = $(SRC_NAME:.c=.o)

SRC = $(addprefix $(SRC_PATH),$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH),$(OBJ_NAME))

all : $(NAME)

$(NAME) : $(OBJ)
	@make -C $(LIBFT)
	@$(CC) $(FLAGS) $(OBJ) $(LIB) -o $(NAME)

$(OBJ) : $(OBJ_PATH)

$(OBJ_PATH) :
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(dir $(OBJ))

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(FLAGS) -I$(HEADERS) -I$(LIBFT)includes -c $< -o $@

clean :
	/bin/rm -rf $(OBJ_PATH)
	make clean -C $(LIBFT)

fclean : clean
	make fclean -C $(LIBFT)
	/bin/rm -f $(NAME)

re : fclean all

