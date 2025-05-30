/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:48:24 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/26 18:25:12 by radubos          ###   ########.fr       */
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
		else if (token->type >= REDIRECT_IN && token->type <= HEREDOC)
			token = token->next;
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
		else if ((*token)->type >= REDIRECT_IN && (*token)->type <= HEREDOC)
			*token = (*token)->next;
		*token = (*token)->next;
	}
	argv[i] = NULL;
	return argv;
}

static t_redir *build_redirections(t_token **token)
{
    t_redir *head = NULL;
    t_redir *current = NULL;

    while (*token && (*token)->type != PIPE)
    {
        if ((*token)->type >= REDIRECT_IN && (*token)->type <= HEREDOC)
        {
            e_redir_type type = (e_redir_type)(*token)->type;
            *token = (*token)->next;
            if (!*token || (*token)->type != WORD)
                return NULL;
            t_redir *new = malloc(sizeof(t_redir));
            new->type = type;
            new->file = ft_strdup((*token)->value);
            new->next = NULL;
            if (!head)
                head = new;
            else
                current->next = new;
            current = new;
        }
        *token = (*token)->next;
    }
    return head;
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
		new_cmd->redirs = build_redirections(&tokens);
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
