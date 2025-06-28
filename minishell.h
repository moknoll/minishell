/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:46:07 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/28 09:16:37 by moritzknoll      ###   ########.fr       */
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
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include "../libft/libft.h"

#define PROMPT "minishell$ "
#define DEBUG 1
#define PATH_MAX 1024

extern int g_exit_status;

// Structs
typedef enum {
	WORD,
	PIPE,
	R_IN,      // <
	R_OUT,     // >
	R_APPEND,  // >>
	HEREDOC           // <<
} e_token_type;

typedef enum {
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
} e_quote_type;

typedef struct s_token_data {
    char *value;
    e_token_type type;
    e_quote_type quote_type;
    int has_space_before;
} t_token_data;

typedef struct s_env {
    char *key;             //  "FOO"
    char *value;           //  "bar"
    int exported;         // If exported
    struct s_env *next;    // Linked list
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
} t_command;

// Tokenizer
t_token		*tokenizer(char *input);
void		print_tokens(t_token *list);
char		*ft_strndup(char *s, size_t n);
//t_token		*new_token(char *value, e_token_type type, e_quote_type quote_type, int has_space_before);
void	add_token(t_token **head, t_token_data data);
void		free_tokens(t_token *head);

// Read line
char		*ft_readline(void);

//
void		builtin(char **argv, t_env **my_env, char **env);
char		**token_to_argv(t_token *token);
int			skip_quotes(const char *input, int i);
char		*strip_quotes(char *str);
void		strip_quotes_inplace(t_token *tokens);
void		expand_tokens(t_token *tokens, t_env *env);
char		*expand_token(t_token *token, t_env *my_env);
void		merge_token(t_token **tokens);
int			set_env(t_env **env, const char *key, const char *value);
char		*get_variable_value_from_env(t_env *my_env, char *var_name);
char		*get_variable_value(char *input, int *i, t_env *my_env);

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
// int fill_command_argv_and_redirs(t_command *cmd, t_token **tok);
int	fill_argv_loop(t_command *cmd, t_token **tok,
	char **argv, t_redir **last_redir);

// Execution
int			execute_external(t_command *cmd_list, char **env, t_env **my_env);
int			apply_redirections(t_redir *redirs);

// Builtin
int	set_env(t_env **env, const char *key, const char *value);
int	ft_env(char **env);
int	ft_unset(t_env **my_env, const char *key);
int	ft_export(t_env *env);
int	handle_export(char **argv, t_env **env);
int	check_multiple_n(char *str);
int	ft_echo_n(char **argv);
int	handle_builtin(char **argv, t_env **my_env);
void	builtin(char **argv, t_env **my_env, char **env);
void	ft_exit(char **argv);
int	set_env(t_env **env, const char *key, const char *value);
int	ft_env_custom(t_env *env);

// Utils
void		append_str(const char *str, char **output);
void		append_char(char c, char **output);
void		free_args(char **argv);
int			is_builtin(char *cmd);
const char	*quote_type_str(e_quote_type qt);
int count_tokens(t_token *tokens);
void free_args(char **argv);
void	free_command_list(t_command *cmd);
void free_env(t_env *env);
void	cleanup(t_token *tokens, t_command *cmd_list, char *line);
int	is_builtin(char *cmd);
const char	*quote_type_str(e_quote_type qt);
t_env	*init_env(t_env **env, char **envp);
int	check_syntax(t_token *tokens);
int execute_builtin_with_redirection(t_command *cmd, t_env **my_env, char **env);
void	handle_heredocs(t_command *cmd_list);
void	setup_heredoc_signals(void);
int	is_operator(char c);

// buitin export tools
void	ft_sort_str_array(char **arr);
char	*ft_strjoin_free(char *s1, const char *s2);
int	env_size(t_env *env);

// Handle signal
void		setup_parent_signals(void);

// token utils
t_token		*new_token(char *value, e_token_type type, e_quote_type quote_type, int has_space_before);
char		**token_to_argv(t_token *token);
void		print_tokens(t_token *tokens);
int	handle_token(char *input, int *i, t_token **tokens, int *has_space);
int	handle_word(char *input, int *i, t_token **tokens, int has_space);
void	handle_operator(char *input, int *i, t_token **tokens, int has_space);

#endif
