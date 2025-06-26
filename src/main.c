/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:48:17 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/25 22:27:01 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(t_env **env, char **envp)
{
	char	*eq;

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

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "cd") == 0 || \
			ft_strcmp(cmd, "echo") == 0 || \
			ft_strcmp(cmd, "pwd") == 0 || \
			ft_strcmp(cmd, "export") == 0 || \
			ft_strcmp(cmd, "unset") == 0 || \
			ft_strcmp(cmd, "env") == 0 || \
			ft_strcmp(cmd, "exit") == 0);
}

static void	process_input(char *line, t_env **my_env, char **env)
{
	t_token		*tokens;
	t_command	*cmd_list;

	tokens = NULL;
	cmd_list = NULL;
	tokens = tokenizer(line);
	if (!tokens)
		return ;
	expand_tokens(tokens, g_exit_status, *my_env);
	merge_token(&tokens);
	strip_quotes_inplace(tokens);
	cmd_list = parse_commands(tokens);
	if (!cmd_list)
	{
		free_tokens(tokens);
		return ;
	}
	if (cmd_list->next == NULL && is_builtin(cmd_list->argv[0]))
		builtin(cmd_list->argv, my_env, env);
	else
		execute_external(cmd_list, env);
	cleanup(tokens, cmd_list, NULL);
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
	(void)argc;
	(void)argv;
	my_env = init_env(&my_env, env);
	setup_parent_signals();
	minishell_loop(&my_env, env);
	free_env(my_env);
	return (g_exit_status);
}
