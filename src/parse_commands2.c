/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:31:11 by radubos           #+#    #+#             */
/*   Updated: 2025/06/26 23:54:34 by radubos          ###   ########.fr       */
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
		else if (token->type >= REDIRECT_IN && token->type <= HEREDOC)
			token = token->next;
		token = token->next;
	}
	return (count);
}

char	**build_argv(t_token **token)
{
    int		argc;
    char	**argv;
    int		i;

    argc = count_args(*token);
    argv = malloc(sizeof(char *) * (argc + 1));
    i = 0;
    if (!argv)
        return (NULL);
    while (*token && (*token)->type != PIPE)
    {
        if ((*token)->type == WORD)
        {
            argv[i++] = ft_strdup((*token)->value);
        }
        else if ((*token)->type >= REDIRECT_IN && (*token)->type <= HEREDOC)
        {
            *token = (*token)->next; // skip redir type
            if (*token)
                *token = (*token)->next; // skip redir file
            continue;
        }
        *token = (*token)->next;
    }
    argv[i] = NULL;
    return (argv);
}
