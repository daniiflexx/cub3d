NAME = raycaster
LEAKS = -fsanitize=address -g3
LIBFT = libft/libft.a
SRCS = src/raycast.c
INC = -Iinc -Ilibft -Imlx_linux -O3
LIB = -Llibft -lft -Lmlx_linux -lmlx_Linux -L/usr/lib -lXext -lX11 -lm -lz
OBJS = $(patsubst src/%, obj/%, $(SRCS:.c=.o))
CFLAGS = -Wall -Wextra -Werror -g
CC = gcc
############################
$(NAME): $(OBJS) $(LIBFT)
		@$(CC) $(CFLAGS)  -g $^ -o $@ $(LIB) $(INC) $(LIB) $(LEAKS)
		@echo "raycaster ready!"

$(LIBFT):
		@make -s bonus -C libft

all: $(NAME)

obj/%.o: src/%.c
		@$(CC) $(CFLAGS) -c -g $< -o $@ $(INC) $(LIB)

clean:
		@rm -f obj/*.o

fclean: clean
		@rm -f $(NAME)
		@make -s fclean -C libft
		@echo "raycaster deleted!"

re: fclean $(NAME)
