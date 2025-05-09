# Compiler and flags
CC      = cc
CFLAGS  = -Wall -Wextra -Werror

# Readline includes and libraries (macOS)
READLINE_FLAGS = -lreadline -lncurses

# DIrectories
SRC_DIR = src
OBJ_DIR = obj

# Sources and executable
SRCS    = $(wildcard $(SRC_DIR)/*.c)
OBJS    = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
NAME    = minishell

# Default rule
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(READLINE_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
