/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/14 13:13:27 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>

/* Global variable for signal handling */
extern int	g_exit_status;

/* Environment variable structure */
typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}	t_env;

/* Token Struct*/
typedef struct s_token_state
{
	int		i;
	int		count;
	int		in_quotes;
	char	quote_char;
}	t_token_state;

/* Expand Struct */
typedef struct s_expand_state
{
	const char	*input;
	int			i;
	char		*output;
	t_env		*env;
	int			in_single_quotes;
	int			in_double_quotes;
}	t_expand_state;

/* Simple data structure for direct execution */
typedef struct s_data
{
	char	*line;
	char	**tokens;
	char	**args;
	t_env	**env;
	char	*path;
}	t_data;

/* Buffer state for reading lines */
typedef struct s_buffer_state
{
	int	pos;
	int	size;
}	t_buffer_state;

/* Range struct*/
typedef struct s_range
{
	int	start;
	int	end;
}	t_range;

/* Line state for reading lines*/
typedef struct s_line
{
	char	*data;
	int		len;
}	t_line_state;

typedef struct s_pipe_commands
{
	char	***commands;
	int		command_count;
	int		**pipes;
	pid_t	*pids;
}	t_pipe_commands;

/* Simple shell functions */
void	minishell_loop(t_env **env);
void	launch_extern_command_simple(t_data *data, t_env *env);
char	*prepare_command_path(char **args, t_env *env);
void	execute_child_process(char **args, char *path, t_env *env);
void	handle_parent_process(pid_t pid, char *path, int *status);
void	execute_pipe_commands(t_data *data, int pipe_count);
void	execute_builtin_with_redirections(t_data *data);

/* Shell main loop */
int		is_empty_line(char *line);

/* Tokenizer functions */
char	**tokenize(char *line);
char	*expand_variables_and_remove_quotes(char *input, t_env *my_env);
char	*heredoc_tmp(void);
void	expand_or_append_character(t_expand_state *state);
int		check_unclosed_quotes(char *line);
void	advance_in_token(char *line, int *i, int *in_quotes, char *quote_char);
void	process_token(char *line, t_token_state *state);
int		count_tokens(char *line);
char	*extract_token(char *line, int *i);
void	skip_whitespace(char *line, int *i);
int		parse_and_add_token(char *line, int *i, char ***tokens, int *count);
char	**prepare_token_array(char *line);
void	split_line_into_tokens(char *line, char **tokens);

/* Redirection functions */
char	*handle_heredoc(char *delimiter);
int		handle_heredoc_redirect(t_data *data, int i);
int		handle_output_redirect(t_data *data, int i, int append);
int		handle_input_redirect(t_data *data, int i);
int		process_redirections(t_data *data);
int		hd_is_end(char *line, char *delim);
void	hd_write(int fd, char *line);
int		create_tmp_heredoc_file(char **filename);
int		should_cleanup_file(int ret);
void	setup_heredoc_signals(struct sigaction *old_int, int *saved);
void	cleanup_heredoc_signals(const struct sigaction *old_int,
			int saved);
void	remove_redirect_args(t_data *data, int i);
int		setup_heredoc_fd(char *heredoc_file, int *saved_stdin);
void	cleanup_heredoc_redirect(int saved_stdin, char *heredoc_file);
int		open_output_file(char *filename, int append);
int		validate_redirect_args(t_data *data, int i);
int		process_single_redirect(t_data *data, int i);
int		fill_buffer(int fd, char *buffer, int *buffer_pos, int *buffer_size);
int		read_character_to_line(char *buffer, int *buffer_pos,
			char *line, int *line_len);
void	finalize_line(char *line, int line_len);
char	*allocate_line_buffer(void);
int		process_line_reading(int fd, char *buffer, t_buffer_state *buf,
			t_line_state *line);

/* Pipe functions */
int		count_pipes(char **args);
char	***split_all_pipe_commands(char **args, int pipe_count);
void	free_commands(char ***commands);
int		process_cmd_redirections(char **cmd);
void	execute_pipe_command(char **cmd, t_env *env, t_data *data,
			t_pipe_commands *pipe_cmds);
void	execute_pipe_chain(char ***commands, int cmd_count,
			t_env *env, t_data *data);
char	*prepare_pipe_command_path(char **cmd, t_env *env, t_data *data,
			t_pipe_commands *pipe_cmds);
void	execute_external_pipe_command(char **cmd, t_env *env, t_data *data,
			t_pipe_commands *pipe_cmds);
int		**create_pipes(int cmd_count);
void	setup_child_pipes(int **pipes, int cmd_count, int i);
void	cleanup_pipes(int **pipes, int cmd_count);
void	wait_for_children(pid_t *pids, int cmd_count);
char	**create_command_from_args(char **args, int start, int end);
void	process_pipe_segment(char ***commands, char **args, int *cmd_index,
			t_range range);
void	remove_redirection_args(char **cmd, int i);
int		handle_output_redirection(char **cmd, int i);
int		handle_append_redirection(char **cmd, int i);
int		handle_input_redirection(char **cmd, int i);

/* Execution functions */
void	execute(t_data *data);

/* Path and execution utilities */
char	*find_command_path(char *cmd, t_env *env);
char	*build_full_path(char *dir, char *cmd);
char	*check_direct_path(char *cmd);
char	*search_in_path_env(char *cmd, char *path_env);

/* Built-in functions */
int		handle_builtin(char **argv, t_env **my_env, t_data *data);
int		is_builtin(char *cmd);
int		ft_cd(char **argv, t_env **env);
int		ft_exit_simple(char **argv, t_env *env, t_data *data);
int		ft_pwd(char **argv);
int		ft_export(t_env *env);
int		handle_unset(char **argv, t_env **my_env);
int		ft_echo(char **argv);
int		ft_echo_n(char **argv);
int		check_multiple_n(char *str);
int		ft_env_custom(t_env *env);
int		handle_export(char **argv, t_env **my_env);
int		handle_unset(char **argv, t_env **my_env);
int		env_size(t_env *env);
t_env	*get_env(t_env *env, const char *key);
void	ft_sort_str_array(char **arr);
int		ft_unset(t_env **my_env, const char *key);
int		ft_env_custom(t_env *env);
void	free_all_and_exit(int exit_code, t_env *env, t_data *data);

/* Environment functions */
t_env	*init_env(t_env **env, char **envp);
char	*get_env_value(t_env *env_list, char *key);
int		set_env_value(t_env **env_list, char *key, char *value);
int		set_env(t_env **env, const char *key, const char *value);
int		unset_env_value(t_env **env_list, char *key);
char	**env_to_array(t_env *env_list);
int		count_exported_vars(t_env *env_list);
int		update_existing_env(t_env *current, char *value);
int		fill_env_array(char **env_array, t_env *env_list, int count);
void	remove_env_node(t_env **env_list, t_env *current, t_env *prev);
t_env	*find_env_node(t_env *env_list, char *key, t_env **prev);
t_env	*create_env_node(const char *key, const char *value);
void	free_env_list(t_env *env_list);

/* Token expansion functions */
char	*extract_and_expand_variable(char *input, int *i, t_env *my_env);
void	concatenate_string(const char *str, char **output);
void	concatenate_character(char c, char **output);
void	expand_dollar_expression(char *input, int *i, char **output,
			t_env *my_env);
char	*get_variable_value_from_env(t_env *env, char *var_name);

/* Signal handling functions */
void	disable_echoctl(void);
void	enable_echoctl(void);
void	hd_set_signals(struct sigaction *old_int);
void	hd_restore_signals(const struct sigaction *old_int);
void	hd_write(int fd, char *line);
int		hd_is_end(char *line, char *delim);
char	*read_line_or_cleanup(int fd, char *buffer,
			t_buffer_state *state, t_line_state *line);
int		heredoc_is_delim(char *line, char *delim, int is_interactive);
char	*heredoc_read_line(int is_interactive);
char	*read_line_simple(int fd);
char	*read_line_interactive(void);

/* Utility functions */
void	print_error(char *cmd, char *msg);
void	handle_status_and_print(int status);

/* Signal handling functions */
void	init_signals_prompt(void);
void	reset_signals_to_default(void);
void	ignore_sigint(void);

int		ft_getchar(void);
void	*ft_realloc(void *ptr, size_t new_size);

#endif
