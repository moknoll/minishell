/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:48:24 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/26 02:31:32 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static e_redir_type	get_redir_type(e_token_type token_type)
{
	if (token_type == REDIRECT_IN)
		return (REDIR_IN);
	else if (token_type == REDIRECT_OUT)
		return (REDIR_OUT);
	else if (token_type == REDIRECT_APPEND)
		return (REDIR_APPEND);
	else
		return (REDIR_HEREDOC);
}

static t_redir	*create_redir_node(e_redir_type type, char *file)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->type = type;
	new->file = ft_strdup(file);
	new->next = NULL;
	new->fd = -1;
	return (new);
}

static int	add_redir_node(t_redir **head, t_redir **current, e_redir_type type,
																	char *file)
{
	t_redir	*new;

	new = create_redir_node(type, file);
	if (!new)
		return (1);
	if (!*head)
		*head = new;
	else
		(*current)->next = new;
	*current = new;
	return (0);
}

static t_redir	*build_redirections(t_token **token)
{
	t_redir			*head;
	t_redir			*current;
	e_redir_type	type;

	head = NULL;
	current = NULL;
	while (*token && (*token)->type != PIPE)
	{
		if ((*token)->type >= REDIRECT_IN && (*token)->type <= HEREDOC)
		{
			type = get_redir_type((*token)->type);
			*token = (*token)->next;
			if (!*token || (*token)->type != WORD)
				return (NULL);
			if (add_redir_node(&head, &current, type, (*token)->value))
				return (NULL);
		}
		*token = (*token)->next;
	}
	return (head);
}

t_command	*parse_commands(t_token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_command	*new_cmd;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		new_cmd = malloc(sizeof(t_command));
		if (!new_cmd)
			return (NULL);
		new_cmd->redirs = build_redirections(&tokens);
		new_cmd->argv = build_argv(&tokens);
		new_cmd->next = NULL;
		if (!head)
			head = new_cmd;
		else
			current->next = new_cmd;
		current = new_cmd;
		if (tokens && tokens->type == PIPE)
			tokens = tokens->next;
	}
	return (head);
}
