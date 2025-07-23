# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: moritz <moritz@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/25 09:08:41 by radubos           #+#    #+#              #
#    Updated: 2025/07/23 12:40:26 by moritz           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g3 -o3

# Readline includes and libraries (macOS)
READLINE_DIR = $(shell brew --prefix readline)
CFLAGS   += -I$(READLINE_DIR)/include
LDFLAGS  += -L$(READLINE_DIR)/lib
LDLIBS   += -lreadline -lcurses

# Libraries
LIBFT_LIB = libft/libft.a

# DIrectories
SRC_DIR = .
OBJ_DIR = obj

# Sources and executable
SRCS    = src/main.c src/builtins/builtins.c src/builtins/builtin_utils.c \
src/builtins/builtin_export.c src/utils/env_list.c src/utils/env_utils.c \
src/utils/env_utils_advanced.c src/utils/signal.c src/utils/signal_hd.c \
src/utils/free_utils.c src/utils/export_utils.c src/parsing/tokenizer.c \
src/parsing/tokenizer_utils.c src/parsing/token_utils.c \
src/parsing/expand_utils.c src/redirections/heredoc.c \
src/redirections/heredoc_read_utils.c src/redirections/heredoc_file_utils.c \
src/redirections/redirections.c src/redirections/redirect_utils.c \
src/redirections/redirect_process.c src/execution/pipes.c \
src/execution/pipe_execution.c src/execution/pipe_utils.c \
src/execution/pipe_redirections.c src/execution/external_commands.c \
src/execution/command_execution.c src/execution/path.c src/execution/path_utils.c \
src/builtins/builtin_env_unset.c src/utils/heredoc_utils.c 

OBJS    = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))
NAME    = minishell

# Default rule
all: $(LIBFT_LIB) $(NAME)

$(LIBFT_LIB):
	make -C libft

$(NAME): $(OBJS) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(OBJS) $(LIBFT_LIB) $(LDLIBS)

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