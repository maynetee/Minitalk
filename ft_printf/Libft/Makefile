NAME = libft.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = $(wildcard srcs/*.c)
OBJS = $(SRCS:.c=.o)

INCLUDES = -I includes

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
