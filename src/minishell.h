/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:46:07 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/14 13:21:37 by moritzknoll      ###   ########.fr       */
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

extern int g_exit_status;

// Structs
typedef enum {
	WORD,
	PIPE,
	REDIRECT_IN,      // <
	REDIRECT_OUT,     // >
	REDIRECT_APPEND,  // >>
	HEREDOC           // <<
} e_token_type;

typedef enum {
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
} e_quote_type;

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

//Utils
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_itoa(int num);
int		ft_isalnum(int c);

// Tokenizer
t_token *tokenizer(char *input);
void    print_tokens(t_token *list);
char	*ft_strndup(char *s, size_t n);
t_token *new_token(char *value, e_token_type type, e_quote_type quote_type, int has_space_before);
void	add_token(t_token **head, char *value, e_token_type type, e_quote_type quote_type, int has_space_before);
void	free_tokens(t_token *head);

// Read line
char	*ft_readline(void);
void builtin(char **argv, t_env **my_env, char **env);
char	**token_to_argv(t_token *token);
void	init_signal(void);
int		skip_quotes(const char *input, int i);
char	*strip_quotes(char *str);
void strip_quotes_inplace(t_token *tokens);
char	*ft_strdup(char *s);
void expand_tokens(t_token *tokens, int g_exit_status, t_env *env);
char *expand_token(t_token *token, int g_exit_status, t_env *my_env);
void merge_token(t_token **tokens);
void set_env(t_env **env, char *arg);
int ft_strcmp(char *s1, char *s2);

#endif
