/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:31:08 by radubos           #+#    #+#             */
/*   Updated: 2025/06/27 10:04:53 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_args(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

int	count_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

void	free_command_list(t_command *cmd)
{
	t_command	*next;

	while (cmd)
	{
		next = cmd->next;
		free_args(cmd->argv);
		free(cmd);
		cmd = next;
	}
}

void	free_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	if (!env)
		return ;
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

void cleanup_all(t_token *tokens, t_command *cmd_list, char *line, t_env** env)
{
	if (env && *env)
	{
		cleanup(tokens, cmd_list, line);
		free_env(*env);
	}
	if (line)
		free(line);
	rl_clear_history(); // optional: readline History aufräumen
}
