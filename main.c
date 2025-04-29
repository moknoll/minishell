/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:48:17 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/04/29 10:25:19 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char *quote_type_str(e_quote_type qt)
{
	if (qt == NO_QUOTE)
		return "NO_QUOTE";
	if (qt == SINGLE_QUOTE)
		return "SINGLE_QUOTE";
	if (qt == DOUBLE_QUOTE)
		return "DOUBLE_QUOTE";
	return "UNKNOWN";
}

void print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: %-10s Type: %d Quote: %s\n",
			tokens->value,
			tokens->type,
			quote_type_str(tokens->quote_type));
		tokens = tokens->next;
	}
}

void free_args(char **argv)
{
	int i ;

	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

int main(void)
{
	char	*line;
	t_token	*tokens;
	char **args;

	// init_signal();
	while(1)
	{
		line = ft_readline();
		if (!line) {
			printf("exit\n");
			break;
		}
		if (*line == '\0') {
			free(line);
			continue;
		}
		tokens = tokenizer(line);
		print_tokens(tokens);
		args = token_to_argv(tokens);
		expand_tokens(tokens, g_exit_status);
		for(int i = 0; args[i]; i++)
		{
			char *stripped = strip_quotes(args[i]);
			free(args[i]);
			args[i] = stripped;
		}

		for (int j = 0; args[j]; j++)
    		printf("argv[%d]: [%s]\n", j, args[j]);
		builtin(args);
		// // execute
		free_tokens(tokens);
		free(line);
		free_args(args);
	}
	return (0);
}

