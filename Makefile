NAME = minishell
CC = cc
CFLAGS = -Wextra -Werror -Wall -std=c99 -g
SRCS = main.c lexer.c parser.c builtin.c signals.c exec.c utils.c
OBJS = $(SRCS:.c=.o)
LIBFT_DIR = libft

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Compiling libft"
	@make -C $(LIBFT_DIR)/
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR)/ -lft -lreadline -o $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning"
	@rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@echo "Full cleaning"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
