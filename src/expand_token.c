/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:48:35 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/27 22:37:14 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

void append_str(const char *str, char **output)
{
    char *new;

    if (!str)
        return;
    if (!*output) {
        *output = ft_strdup(str);
        return;
    }
    new = ft_strjoin(*output, str);
    free(*output);
    *output = new;
}

void append_char(char c, char **output)
{
	char temp[2]= {c, '\0'};
	append_str(temp, output);
}

char *get_variable_value_from_env(t_env *my_env, char *var_name)
{
    if (!var_name)
    {
        return (NULL);
    }
	while (my_env)
	{
		if (ft_strcmp(my_env->key, var_name) == 0 && my_env->exported)
			return ft_strdup(my_env->value); // eigene Kopie
		my_env = my_env->next;
	}
	return ft_strdup(""); // nicht gefunden
}

char *get_variable_value(char *input, int *i, t_env *my_env)
{
	int start;
	char *var_name;
	char *var_value;

	start = *i;
	// Move i forward to the end od the variale name
	while(input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	var_name = ft_strndup(&input[start], *i - start);
	var_value = get_variable_value_from_env(my_env ,var_name);
	free(var_name);
	if(!var_value)
		var_value = "";
	return(ft_strdup(var_value));
}

void handle_dollar(char *input, int *i, char **output, int g_exit_status, t_env *my_env)
{
	char *exit_status_str;
	char *var_value;

	(*i)++;
	if(input[*i] == '?')
	{
		exit_status_str = ft_itoa(g_exit_status);
		append_str(exit_status_str, output);
		free(exit_status_str);
		(*i)++;
	}
    else if (ft_isalpha(input[*i]) || input[*i] == '_')
    {
        while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
            (*i)++;
        var_value = get_variable_value(input, i, my_env);
        append_str(var_value, output);
        free(var_value);
    }
	else
		append_char('$', output);
}

char *expand_token(t_token *token, int g_exit_status, t_env *my_env)
{
	const char *input = token->value;
    char    *output = NULL;
	int i = 0;
    
    if (!input)
        return (NULL);
    if (token->quote_type == SINGLE_QUOTE)
		return ft_strdup(token->value); // No expansion in single quotes
    output = calloc(1, 1); // Start with empty string
	while (input[i])
	{
		if (input[i] == '$' && token->quote_type != SINGLE_QUOTE)
			handle_dollar((char *)input, &i, &output, g_exit_status, my_env);
		else
		{
			append_char(input[i], &output);
			i++;
		}
	}
	return output;
}

void expand_tokens(t_token *tokens, int g_exit_status, t_env *env)
{
	t_token *current;
	char *expanded_value;

	current = tokens;
	while(current)
	{
		if(current->type == WORD)
		{
			printf("Before: [%s] Quote: %d\n", current->value, current->quote_type);
			expanded_value = expand_token(current, g_exit_status, env);
            if (!expanded_value)
            {
                current = current->next;
                continue;
            }
			free(current->value);
			current->value = expanded_value;
			printf("After:  [%s]\n", current->value);
		}
		current = current->next;
	}
}


