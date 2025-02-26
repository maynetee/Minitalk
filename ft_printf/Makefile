NAME = libftprintf.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = $(wildcard srcs/*.c)
OBJS = $(SRCS:.c=.o)

INCLUDES = -I includes -I $(LIBFT_DIR)/includes

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	ar rcs $(NAME) $(OBJS) $(LIBFT_DIR)/srcs/*.o

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all
