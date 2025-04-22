/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:46:07 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/04/22 14:17:18 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

// Structs
typedef enum {
	WORD,
	PIPE,
	REDIRECT_IN,      // <
	REDIRECT_OUT,     // >
	REDIRECT_APPEND,  // >>
	HEREDOC           // <<
} e_token_type;

typedef struct s_token {
	char *value;
	e_token_type type;
	struct s_token *next;
} t_token;

// Tokenizer
t_token *tokenizer(char *input);
void    print_tokens(t_token *list);
char	*ft_strndup(char *s, size_t n);
t_token *new_token(char *value, e_token_type type);
void add_token(t_token **head, char *value, e_token_type type);
void free_tokens(t_token *head);

// Read line
char	*ft_readline(void);

#endif
