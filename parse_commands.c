/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:48:24 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/27 11:32:58 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_args(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == WORD)
			count++;
		else if (token->type >= R_IN && token->type <= HEREDOC)
			token = token->next;
		if (token)
			token = token->next;
	}
	return (count);
}

int	fill_command_argv_and_redirs(t_command *cmd, t_token **tok)
{
	t_redir	*last_redir;
	int		argc;
	char	**argv;

	last_redir = NULL;
	argc = count_args(*tok);
	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (0);
	return (fill_argv_loop(cmd, tok, argv, &last_redir));
}

t_command	*build_command(t_token **tokens)
{
	t_command	*cmd;
	t_token		*tok;

	tok = *tokens;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	if (!fill_command_argv_and_redirs(cmd, &tok))
	{
		free(cmd);
		return (NULL);
	}
	*tokens = tok;
	return (cmd);
}

t_command	*parse_commands(t_token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_command	*cmd;

	current = NULL;
	head = NULL;
	while (tokens)
	{
		cmd = build_command(&tokens);
		if (!cmd)
			return (NULL);
		if (!head)
			head = cmd;
		else
			current->next = cmd;
		current = cmd;
		if (tokens && tokens->type == PIPE)
			tokens = tokens->next;
	}
	return (head);
}
