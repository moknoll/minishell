/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:48:35 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/06 10:43:44 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

/*Append a string to an existing outpit, We'll dynamically allocate space for the output as needed */

// test
void append_str(char *str, char **output)
{
	char *new_output;

	if (!str)
		return;
	if (!*output)
		*output = ft_strdup(str);
	else
	{
		new_output = ft_strjoin(*output, str);
		free(*output);
		*output = new_output;
	}
}

void append_char(char c, char **output)
{
	char temp[2]= {c, '\0'};
	append_str(temp, output);
}

char *get_variable_value(char *input, int *i)
{
	int start;
	char *var_name;
	char *var_value;

	start = *i;
	// Move i forward to the end od the variale name
	while(input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	var_name = ft_strndup(&input[start], *i - start);
	var_value = getenv(var_name);
	free(var_name);
	if(!var_value)
		var_value = "";
	return(ft_strdup(var_value));
}

void handle_dollar(char *input, int *i, char **output, int g_exit_status)
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
	else
	{
		var_value = get_variable_value(input, i);
		append_str(var_value, output);
		free(var_value);
	}
}

char *expand_token(t_token *token, int g_exit_status)
{
	if (token->quote_type == SINGLE_QUOTE)
		return ft_strdup(token->value); // No expansion in single quotes

	const char *input = token->value;
	char *output = calloc(1, 1); // Start with empty string
	int i = 0;

	while (input[i])
	{
		if (input[i] == '$')
			handle_dollar((char *)input, &i, &output, g_exit_status);
		else
		{
			append_char(input[i], &output);
			i++;
		}
	}
	return output;
}

void expand_tokens(t_token *tokens, int g_exit_status)
{
	t_token *current;
	char *expanded_value;

	current = tokens;
	while(current)
	{
		if(current->type == WORD)
		{
			printf("Before: [%s] Quote: %d\n", current->value, current->quote_type);
			expanded_value = expand_token(current, g_exit_status);
			free(current->value);
			current->value = expanded_value;
			printf("After:  [%s]\n", current->value);
		}
		current = current->next;
	}

}
