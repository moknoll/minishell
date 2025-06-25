/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:46:07 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/25 20:07:44 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include "../libft/libft.h"

#define PROMPT "minishell$ "
#define DEBUG 1

extern int g_exit_status;

// Structs
typedef enum {
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	HEREDOC
} e_token_type;

typedef enum {
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
} e_quote_type;

typedef struct s_env {
    char *key;
    char *value;
    int exported;
    struct s_env *next;
} t_env;

typedef struct s_token {
	char *value;
	e_token_type type;
	e_quote_type quote_type;
	struct s_token *next;
	int has_space_before;
} t_token;

typedef enum {
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} e_redir_type;

typedef struct s_redir {
    e_redir_type type;
    char *file;
    struct s_redir *next;
    int fd;
} t_redir;

typedef struct s_command {
	char **argv;
	struct s_command *next;
	t_redir *redirs;
	int heredoc_fd;
} t_command;

// Tokenizer
t_token		*tokenizer(char *input);
void		print_tokens(t_token *list);
char		*ft_strndup(char *s, size_t n);
t_token		*new_token(char *value, e_token_type type, e_quote_type quote_type, int has_space_before);
void		add_token(t_token **head, char *value, e_token_type type, e_quote_type quote_type, int has_space_before);
void		free_tokens(t_token *head);

// Read line
char		*ft_readline(void);

//
void		builtin(char **argv, t_env **my_env, char **env);
char		**token_to_argv(t_token *token);
int			skip_quotes(const char *input, int i);
char		*strip_quotes(char *str);
void		strip_quotes_inplace(t_token *tokens);
void		expand_tokens(t_token *tokens, int g_exit_status, t_env *env);
char		*expand_token(t_token *token, int g_exit_status, t_env *my_env);
void		merge_token(t_token **tokens);
int			set_env(t_env **env, const char *key, const char *value);
char		*get_variable_value_from_env(t_env *my_env, char *var_name);

// pipe
int			create_pipe(t_command *cmd, int pipefd[2]);
void		child_process(t_command *cmd, int prev_fd, int pipefd[2]);
void		ft_free_tab(char **tab);
char		*get_env_path(char *env[]);
char		**split_path(char *envp[]);
char		*get_path(char *cmd, char *envp[]);
void		execute_child_process(t_command *cmd, char **env);
int			parent_cleanup_and_wait(pid_t pid, int *prev_fd, int pipefd[2], t_command *cmd);
void		ft_pipe(t_command *cmd_list, char *env[]);

// parse commands
t_command	*parse_commands(t_token *tokens);

// Execution
void		execute_external(t_command *cmd_list, char **env);
int			apply_redirections(t_redir *redirs);

// Builtin


// Utils
void		append_str(const char *str, char **output);
void		append_char(char c, char **output);
void		free_args(char **argv);
int			is_builtin(char *cmd);
const char	*quote_type_str(e_quote_type qt);
char		*ft_realloc(char * src, int old_size, int new_size);
char		*ft_malloc(char *src, int size);

// Handle signal
void		setup_parent_signals(void);
void    	setup_child_signals(void);
void    	setup_heredoc_signals(void);

// Heredoc handled in apply_redirections()

// token utils
t_token		*new_token(char *value, e_token_type type, e_quote_type quote_type, int has_space_before);
void		add_token(t_token **head, char *value, e_token_type type, e_quote_type quote_type, int has_space_before);
void		free_tokens(t_token *head);
char		**token_to_argv(t_token *token);
void		print_tokens(t_token *tokens);
int			token_list_size(t_token **token);

// Preprocess heredoc redirections before forking
void    handle_heredocs(t_command *cmd_list);

#endif
