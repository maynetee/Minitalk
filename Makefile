NAME = client server
CC = gcc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

client: client.c $(LIBFT)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR)/includes client.c $(LIBFT) -o client

server: server.c $(LIBFT)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR)/includes server.c $(LIBFT) -o server

bonus: $(LIBFT) client_bonus server_bonus

client_bonus: client_bonus.c $(LIBFT)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR)/includes client_bonus.c $(LIBFT) -o client_bonus

server_bonus: server_bonus.c $(LIBFT)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR)/includes server_bonus.c $(LIBFT) -o server_bonus

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f *.o

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f client server client_bonus server_bonus

re: fclean all

.PHONY: all clean fclean re bonus
