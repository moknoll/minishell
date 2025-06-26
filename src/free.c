/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 22:05:29 by radubos           #+#    #+#             */
/*   Updated: 2025/06/26 00:52:46 by radubos          ###   ########.fr       */
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

void	cleanup(t_token *tokens, t_command *cmd_list, char *line)
{
	if (tokens)
		free_tokens(tokens);
	if (cmd_list)
		free_command_list(cmd_list);
	if (line)
		free(line);
}

void	ft_free_tab(char **tab)
{
	int	i;

	if (!tab)
		perror("no tab");
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}
