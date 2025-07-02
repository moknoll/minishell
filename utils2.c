/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 07:31:10 by mknoll            #+#    #+#             */
/*   Updated: 2025/06/27 12:15:07 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*quote_type_str(e_quote_type qt)
{
	if (qt == NO_QUOTE)
		return ("NO_QUOTE");
	if (qt == SINGLE_QUOTE)
		return ("SINGLE_QUOTE");
	if (qt == DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	return ("UNKNOWN");
}

t_env	*init_env(t_env **env, char **envp)
{
	char	*eq;
	char	*key;
	char	*value;

	while (*envp)
	{
		eq = ft_strchr(*envp, '=');
		if (eq)
		{
			key = ft_substr(*envp, 0, eq - *envp);
			value = ft_strdup(eq + 1);
			set_env(env, key, value);
			free(key);
			free(value);
		}
		envp++;
	}
	return (*env);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

void	cleanup(t_token *tokens, t_command *cmd_list, char *line)
{
	if (tokens)
		free_tokens(tokens);
	if (cmd_list)
		free_command_list(cmd_list);
	if (line)
		free(line);
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		if (head->value)
			free(head->value);
		free(head);
		head = tmp;
	}
}
