# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: radubos <radubos@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/25 09:08:41 by radubos           #+#    #+#              #
#    Updated: 2025/08/04 14:36:03 by radubos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g3
#-----------------------------only for mac
READLINE_DIR = $(shell brew --prefix readline)
CFLAGS   += -I$(READLINE_DIR)/include
LDFLAGS  += -L$(READLINE_DIR)/lib
LDLIBS   += -lreadline -lcurses
#-----------------------------delete it before push and uncomment what is next
#LDLIBS   += -lreadline

# Libraries
LIBFT_LIB = libft/libft.a

# Directories
SRC_DIR = .
OBJ_DIR = obj

# Sources and executable
SRCS    = src/main.c src/builtins/builtins.c src/builtins/builtin_utils.c \
src/builtins/builtin_export.c src/utils/env_list.c src/utils/env_utils.c \
src/utils/env_utils_advanced.c src/utils/signal.c src/utils/signal_hd.c \
src/utils/export_utils.c src/tokenize/tokenizer.c \
src/tokenize/tokenizer_utils.c src/tokenize/token_expansion.c \
src/tokenize/variable_expansion.c src/redirections/heredoc.c \
src/redirections/heredoc_read_utils.c src/redirections/heredoc_file_utils.c \
src/redirections/redirections.c src/redirections/redirect_utils.c \
src/redirections/redirect_process.c src/execution/pipes.c \
src/execution/pipe_execution.c src/execution/pipe_utils.c \
src/execution/pipe_redirections.c src/execution/external_commands.c \
src/execution/command_execution.c src/execution/path.c \
src/builtins/builtin_env_unset.c src/utils/heredoc_utils.c src/utils/more_hd_utils.c \

OBJS    = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))
NAME    = minishell

# Default rule
all: $(LIBFT_LIB) $(NAME)

$(LIBFT_LIB):
	make -C libft

$(NAME): $(OBJS) $(LIBFT_LIB)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT_LIB) $(LDLIBS) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Iincludes -Ilibft -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re