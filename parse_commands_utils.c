/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 10:01:52 by mknoll            #+#    #+#             */
/*   Updated: 2025/06/27 11:42:45 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static e_redir_type	map_token_to_redir_type(e_token_type token_type)
{
	if (token_type == R_IN)
		return (REDIR_IN);
	else if (token_type == R_OUT)
		return (REDIR_OUT);
	else if (token_type == R_APPEND)
		return (REDIR_APPEND);
	else if (token_type == HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_IN);
}

static void	cleanup_argv(char **argv, int count)
{
	while (--count >= 0)
		free(argv[count]);
	free(argv);
}

static int	handle_redirection(t_command *cmd,
		t_token **tok, t_redir **last_redir)
{
	e_redir_type	rtype;
	t_redir			*r;

	rtype = map_token_to_redir_type((*tok)->type);
	*tok = (*tok)->next;
	if (!*tok || (*tok)->type != WORD)
		return (0);
	r = malloc(sizeof(t_redir));
	if (!r)
		return (0);
	r->type = rtype;
	r->file = ft_strdup((*tok)->value);
	if (!r->file)
	{
		free(r);
		return (0);
	}
	r->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = r;
	else
		(*last_redir)->next = r;
	*last_redir = r;
	return (1);
}

int	fill_argv_loop(t_command *cmd, t_token **tok,
		char **argv, t_redir **last_redir)
{
	int	i;

	i = 0;
	while (*tok && (*tok)->type != PIPE)
	{
		if ((*tok)->type == WORD)
		{
			argv[i] = ft_strdup((*tok)->value);
			if (!argv[i])
				return (cleanup_argv(argv, i), 0);
			i++;
		}
		else if ((*tok)->type >= R_IN && (*tok)->type <= HEREDOC)
		{
			if (!handle_redirection(cmd, tok, last_redir))
				return (cleanup_argv(argv, i), 0);
		}
		*tok = (*tok)->next;
	}
	argv[i] = NULL;
	cmd->argv = argv;
	return (1);
}
