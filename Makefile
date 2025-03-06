NAME = client server
CC = clang
CFLAGS = -Wall -Wextra -Werror
LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC_CLIENT = client.c
SRC_SERVER = server.c
OBJ_CLIENT = $(SRC_CLIENT:.c=.o)
OBJ_SERVER = $(SRC_SERVER:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

client: $(OBJ_CLIENT) $(LIBFT)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR)/includes $(OBJ_CLIENT) $(LIBFT) -o client

server: $(OBJ_SERVER) $(LIBFT)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR)/includes $(OBJ_SERVER) $(LIBFT) -o server

bonus: all

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJ_CLIENT) $(OBJ_SERVER)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f client server

re: fclean all

.PHONY: all clean fclean re bonus
