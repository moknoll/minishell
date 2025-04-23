# Compiler and flags
CC      = cc
CFLAGS  = -Wall -Wextra -Werror

# Readline includes and libraries (macOS)
READLINE_FLAGS = -lreadline -lncurses

# Sources and executable
SRCS    = main.c ft_readline.c token_utils.c tokenizer.c builtin.c handle_signal.c
OBJS    = $(SRCS:.c=.o)
NAME    = minishell

# Default rule
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(READLINE_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
