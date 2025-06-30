/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:48:17 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/28 09:16:23 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*tokenize_and_check(char *line)
{
	t_token	*tokens;

	tokens = tokenizer(line);
	if (!tokens)
		return (NULL);
	if (!check_syntax(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}

static t_command	*prepare_commands(t_token *tokens, t_env **my_env)
{
	expand_tokens(tokens, *my_env);
	merge_token(&tokens);
	strip_quotes_inplace(tokens);
	return (parse_commands(tokens));
}

static void	process_input(char *line, t_env **my_env, char **env)
{
	t_token		*tokens;
	t_command	*cmd_list;

	tokens = tokenize_and_check(line);
	if (!tokens)
		return ;
	cmd_list = prepare_commands(tokens, my_env);
	if (!cmd_list)
	{
		free_tokens(tokens);
		return ;
	}
	if (cmd_list->next == NULL && is_builtin(cmd_list->argv[0]))
	{
		if (!execute_builtin_with_redirection(cmd_list, my_env, env, tokens))
			return ;
	}
	else
		execute_external(cmd_list, env, my_env);
	cleanup(tokens, cmd_list, line);
}

static void	minishell_loop(t_env **my_env, char **env)
{
	char	*line;

	while (1)
	{
		line = ft_readline();
		if (!line)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		process_input(line, my_env, env);
		free(line);
	}
}

int	main(int argc, char *argv[], char *env[])
{
	t_env	*my_env;

	my_env = NULL;
	(void)argc, (void)argv;
	my_env = init_env(&my_env, env);
	setup_parent_signals();
	minishell_loop(&my_env, env);
	free_env(my_env);
	return (g_exit_status);
}
