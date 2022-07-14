NAME = test_mlx
MLX = libmlx.dylib

D_SRC = ./
D_MLX = ./mlx_mms

SRC = main.c
OBJ = $(SRC:.c=.o)

INC = -I$(D_MLX)
LIB = -L$(D_MLX) -lmlx

CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

ifeq ($(USE_IMG), 1)
CFLAGS += -D USE_IMG_BUF
endif

.PHONY: all clean fclean re

test:
	@$(MAKE) -s re
	@echo "----- put texture to window ------"
	./$(NAME)
	@$(MAKE) -s re USE_IMG=1
	@echo "----- put texture to img buffer ------"
	./$(NAME)

all: $(NAME)

$(NAME): $(MLX)
	$(CC) $(CFLAGS) $(INC) $(LIB) -o $@ $(SRC)

$(MLX):
	@$(MAKE) -s -C $(D_MLX) all
	@ln -sf $(PWD)/$(D_MLX)/$(MLX) $(PWD)

clean:
	$(RM) -r $(OBJ)

mlx_clean:
	$(MAKE) -sC $(D_MLX) clean

fclean: clean
	$(RM) $(NAME) $(MLX)

re: fclean all
