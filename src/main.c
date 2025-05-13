/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:48:17 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/13 13:20:26 by moritzknoll      ###   ########.fr       */
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

// void pipe_and_redirection(t_token **tokens)
// {
// 	e_token_type token_type;

// 	if(token_type == PIPE)
// 		ft_pipe(token_to_argv(*tokens));
// 	else if(token_type == HEREDOC)
// 		ft_heredoc();
// 	else if (token_type == REDIRECT_IN)
// 		ft_redirect_in();
// 	else if (token_type == REDIRECT_OUT)
// 		ft_redirect_out();
// 	else if (token_type == REDIRECT_APPEND)
// 		ft_redirect_append();
// 	else
// 		return ;

// }

int main(int argc, char *argv[], char *env[])
{
    char *line;
    t_token *tokens;
    char **args;


	(void)argc;
	(void)argv;
    // init_signal();
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
        // Tokenize the input line (adds tokens with correct quoting info)
        tokens = tokenizer(line);
        if(!tokens)
        {
            free(line);
            continue;
        }

        // Print raw tokens for debug
        print_tokens(tokens);

        // Step 1: Expand tokens in place
        expand_tokens(tokens, g_exit_status);

        // Step 2: Merge tokens where needed (no space between)
        merge_token(&tokens);

        // Step 3: Strip quotes after merging
        strip_quotes_inplace(tokens);

        // Step 4: Convert to argv[]
        int token_count = count_tokens(tokens);
        args = malloc((token_count + 1) * sizeof(char *));

        t_token *tmp = tokens;
        int i = 0;
        while (tmp)
        {
            args[i++] = ft_strdup(tmp->value); // already stripped & merged
            tmp = tmp->next;
        }
        args[i] = NULL;

        // Null-terminate the args array
        args[i] = NULL;
        // Print the final arguments array for debugging
        for (int j = 0; args[j]; j++)
        {
            printf("argv[%d]: [%s]\n", j, args[j]);
        }
        // Call builtin function (or execution logic)
        builtin(args, env);
        // Clean up memory
        free_tokens(tokens);
        free(line);
        free_args(args); // Ensure that args is properly freed after usage
    }
    return (0);
}


