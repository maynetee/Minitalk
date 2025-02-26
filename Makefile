NAME = client server
CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC_CLIENT = client.c
SRC_SERVER = server.c
SRC_CLIENT_BONUS = client_bonus.c
SRC_SERVER_BONUS = server_bonus.c

all: $(NAME)

$(NAME): client server

client: $(SRC_CLIENT)
	$(CC) $(CFLAGS) $(SRC_CLIENT) -o client

server: $(SRC_SERVER)
	$(CC) $(CFLAGS) $(SRC_SERVER) -o server

bonus: client_bonus server_bonus

client_bonus: $(SRC_CLIENT_BONUS)
	$(CC) $(CFLAGS) $(SRC_CLIENT_BONUS) -o client_bonus

server_bonus: $(SRC_SERVER_BONUS)
	$(CC) $(CFLAGS) $(SRC_SERVER_BONUS) -o server_bonus

clean:
	rm -f *.o

fclean: clean
	rm -f client server client_bonus server_bonus

re: fclean all

.PHONY: all clean fclean re bonus
