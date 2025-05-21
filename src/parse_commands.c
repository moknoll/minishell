/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:48:24 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/21 10:41:14 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int count_args(t_token *token)
{
	int count;

	count = 0;
	while(token && token->type != PIPE)
	{
		if(token->type == WORD)
			count++;
		token = token->next;
	}
	return (count);
}

static char **build_argv(t_token **token)
{
	int argc;
	char **argv;
	int i;

	argc = count_args(*token);
	argv = malloc(sizeof(char *) * (argc + 1));
	i = 0;
	if(!argv)
		return NULL;
	while(*token && (*token)->type != PIPE)
	{
		if((*token)->type == WORD)
			argv[i++] = ft_strdup((*token)->value);
		*token = (*token)->next;
	}
	argv[i] = NULL;
	return argv;
}

t_command *parse_commands(t_token *tokens)
{
	t_command *head;
	t_command *current;
	t_command *new_cmd;

	head = NULL;
	current = NULL;
	while(tokens)
	{
		new_cmd = malloc(sizeof(t_command));
		if(!new_cmd)
			return NULL;
		new_cmd->argv = build_argv(&tokens);
		new_cmd->next = NULL;
		if(!head)
			head = new_cmd;
		else
			current->next = new_cmd;
		current = new_cmd;
		if(tokens && tokens->type == PIPE)
			tokens = tokens->next;
	}
	return (head);
}
