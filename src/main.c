/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:48:17 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/22 12:06:20 by moritzknoll      ###   ########.fr       */
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

t_env *init_env(t_env **env, char **envp)
{
    int i = 0;
    while (envp[i])
    {
        set_env(env, envp[i]);  // Reuse deine Funktion, z. B. "PATH=/bin"
        i++;
    }
    return *env;
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

void free_command_list(t_command *cmd)
{
    while (cmd)
    {
        t_command *next = cmd->next;
        free_args(cmd->argv);
        free(cmd);
        cmd = next;
    }
}

int is_builtin(char *cmd)
{
    return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
         || ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "env") == 0
         || ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "export") == 0
         || ft_strcmp(cmd, "unset") == 0);
}


int main(int argc, char *argv[], char *env[])
{
    char *line;
    t_token *tokens;
    t_env *my_env = NULL;

    my_env = init_env(&my_env, env);
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
        expand_tokens(tokens, g_exit_status, my_env);

        // Step 2: Merge tokens where needed (no space between)
        merge_token(&tokens);

        // Step 3: Strip quotes after merging
        strip_quotes_inplace(tokens);

       // Step 4: Parse tokens into commands
        t_command *cmd_list = parse_commands(tokens);
        if (cmd_list && !cmd_list->next) // Nur 1 Command = kein Pipe
        {
            if (is_builtin(cmd_list->argv[0]))
                builtin(cmd_list->argv, &my_env, env);
            else
                 execute_external(cmd_list, env);
        }
        else
            ft_pipe(cmd_list, env);

        // Free everything
        free_command_list(cmd_list);  // ← musst du implementieren

        // Clean up memory
        free_tokens(tokens);
        free(line);
    }
    return (0);
}


