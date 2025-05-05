/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:48:17 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/05 16:12:45 by moritzknoll      ###   ########.fr       */
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

int count_tokens(t_token *tokens)
{
    int count = 0;
    while (tokens)
    {
        count++;
        tokens = tokens->next;
    }
    return count;
}

int main(void)
{
    char *line;
    t_token *tokens;
    char **args;

    init_signal();
    while (1)
    {
        line = ft_readline();
        if (!line)
        {
            printf("exit\n");
            break;
        }
        if (*line == '\0')
        {
            free(line);
            continue;
        }

        // Tokenize the input line
        tokens = tokenizer(line);
        // merge_token(&tokens);

        // Print the tokens for debugging (can be removed later)
        print_tokens(tokens);

        // Expand the tokens based on $variable and $? handling
        expand_tokens(tokens, g_exit_status);

        // Convert tokens to an argv-style array
        // Counting the number of tokens first to allocate memory
        int token_count = count_tokens(tokens);
        args = malloc((token_count + 1) * sizeof(char *));

        t_token *tmp = tokens;
        int i = 0;

        // Iterate through the tokens to handle expansion and strip quotes
        while (tmp)
        {
            // Expand the token value (handle $USER, $? etc.)
            char *expanded = expand_token(tmp, g_exit_status);
            // Strip quotes if necessary
            char *stripped = strip_quotes(expanded);
            // Free the expanded value since we don't need it anymore
            free(expanded);
            // Store the stripped token in the args array
            args[i++] = stripped;
            tmp = tmp->next; // Move to the next token
        }
        // Null-terminate the args array
        args[i] = NULL;
        // Print the final arguments array for debugging
        for (int j = 0; args[j]; j++)
        {
            printf("argv[%d]: [%s]\n", j, args[j]);
        }
        // Call builtin function (or execution logic)
        builtin(args);
        // Clean up memory
        free_tokens(tokens);
        free(line);
        free_args(args); // Ensure that args is properly freed after usage
    }
    return (0);
}


