/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:48:17 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/27 22:32:51 by radubos          ###   ########.fr       */
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

void free_args(char **argv)
{
    int i;

    if (!argv)
        return; 
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

/* t_env *init_env(t_env **env, char **envp)
{
    int i = 0;
    while (envp[i])
    {
        set_env(env, envp[i]);  // Reuse deine Funktion, z. B. "PATH=/bin"
        i++;
    }
    return *env;
} */
t_env *init_env(t_env **env, char **envp) 
{
    char *eq;
    
    while (*envp)
    {
        eq = ft_strchr(*envp, '=');
        if (eq)
        {
            *eq = '\0';
            set_env(env, *envp, eq + 1);
            *eq = '=';
        }
        envp++;
    }
    return (*env);
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
    if (!cmd)
        return (0);   
    return (ft_strcmp(cmd, "cd") == 0 || 
            ft_strcmp(cmd, "echo") == 0 ||
            ft_strcmp(cmd, "pwd") == 0 ||
            ft_strcmp(cmd, "export") == 0 ||
            ft_strcmp(cmd, "unset") == 0 ||
            ft_strcmp(cmd, "env") == 0 ||
            ft_strcmp(cmd, "exit") == 0);
}

static void cleanup(t_token *tokens, t_command *cmd_list, char *line)
{
    if (tokens)
        free_tokens(tokens);
    if (cmd_list)
        free_command_list(cmd_list);
    if (line)
        free(line);
}

static void print_debug_info(t_token *tokens, t_command *cmd_list)
{
    #ifdef DEBUG
    // Debug: print tokens
    printf("\n--- Tokens ---\n");
    print_tokens(tokens);
    
    // Debug: print commands
    printf("\n--- Commands ---\n");
    t_command *current = cmd_list;
    while (current)
    {
        printf("Command:\n");
        for (int j = 0; current->argv && current->argv[j]; j++)
            printf("  argv[%d]: [%s]\n", j, current->argv[j]);

        t_redir *r = current->redirs;
        while (r)
        {
            printf("  redir: type=%d file=%s\n", r->type, r->file);
            r = r->next;
        }
        current = current->next;
    }
    printf("\n");
    #endif
}

static void process_input(char *line, t_env **my_env, char **env)
{
    t_token *tokens = NULL;
    t_command *cmd_list = NULL;

    tokens = tokenizer(line);
    if (!tokens)
        return;
    expand_tokens(tokens, g_exit_status, *my_env);
    merge_token(&tokens);
    strip_quotes_inplace(tokens);
    cmd_list = parse_commands(tokens);
    if (!cmd_list)
    {
        free_tokens(tokens);
        return;
    }
    print_debug_info(tokens, cmd_list);
    if (cmd_list->next == NULL && is_builtin(cmd_list->argv[0]))
        builtin(cmd_list->argv, my_env, env);
    else
        execute_external(cmd_list, env);
    cleanup(tokens, cmd_list, NULL);
}

void free_env(t_env *env)
{
    t_env *current = env;
    t_env *next;

    if (!env)
        return;
    while (current)
    {
        next = current->next;
        if (current->key)
        {
            free(current->key);
            current->key = NULL;
        }
        if (current->value)
        {
            free(current->value);
            current->value = NULL;
        }
        free(current);
        current = next;
    }
}

/* int main(int argc, char *argv[], char *env[])
{
    char *line;
    t_token *tokens;
    t_env *my_env = NULL;

    my_env = init_env(&my_env, env);
	(void)argc;
	(void)argv;
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
} */

int main(int argc, char *argv[], char *env[])
{
    char *line;
    t_env *my_env = NULL;

    (void)argc;
    (void)argv;
    my_env = init_env(&my_env, env);
    setup_parent_signals();
    while (1)
    {
        line = readline(PROMPT);
        if (!line)
        {
            ft_putendl_fd("exit", STDOUT_FILENO);
            break;
        }
        if (*line == '\0')
        {
            free(line);
            continue;
        }
        add_history(line);
        process_input(line, &my_env, env);
        free(line);
    }
    free_env(my_env);
    return (g_exit_status);
}


