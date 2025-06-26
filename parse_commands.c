/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:48:24 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/26 11:59:51 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static e_redir_type map_token_to_redir_type(e_token_type token_type)
{
    if (token_type == REDIRECT_IN)
        return REDIR_IN;
    else if (token_type == REDIRECT_OUT)
        return REDIR_OUT;
    else if (token_type == REDIRECT_APPEND)
        return REDIR_APPEND;
    else if (token_type == HEREDOC)
        return REDIR_HEREDOC;
    // Optional: Fehlerfall
    return REDIR_IN;
}

static int count_args(t_token *token)
{
    int count = 0;

    while (token && token->type != PIPE)
    {
        if (token->type == WORD)
            count++;
        else if (token->type >= REDIRECT_IN && token->type <= HEREDOC)
            token = token->next;  // Skip Datei-Token hinter der Redirection

        if (token)
            token = token->next;
    }
    return count;
}


int fill_command_argv_and_redirs(t_command *cmd, t_token **tok)
{
    t_redir *last_redir = NULL;
    int argc = count_args(*tok);
    char **argv = malloc(sizeof(char *) * (argc + 1));
    int i = 0;

    if (!argv)
        return 0;

    while (*tok && (*tok)->type != PIPE)
    {
        if ((*tok)->type == WORD)
        {
            argv[i++] = ft_strdup((*tok)->value);
            if (!argv[i-1])
            {
                // Fehler: Speicher freigeben
                while (--i >= 0)
                    free(argv[i]);
                free(argv);
                return 0;
            }
        }
        else if ((*tok)->type >= REDIRECT_IN && (*tok)->type <= HEREDOC)
        {
            e_redir_type rtype = map_token_to_redir_type((*tok)->type);
            *tok = (*tok)->next;
            if (!*tok || (*tok)->type != WORD)
            {
                // Cleanup argv
                while (--i >= 0)
                    free(argv[i]);
                free(argv);
                return 0;
            }
            t_redir *r = malloc(sizeof(t_redir));
            if (!r)
            {
                // Cleanup argv
                while (--i >= 0)
                    free(argv[i]);
                free(argv);
                return 0;
            }
            r->type = rtype;
            r->file = ft_strdup((*tok)->value);
            if (!r->file)
            {
                free(r);
                while (--i >= 0)
                    free(argv[i]);
                free(argv);
                return 0;
            }
            r->next = NULL;
            if (!cmd->redirs)
                cmd->redirs = r;
            else
                last_redir->next = r;
            last_redir = r;
        }
        *tok = (*tok)->next;
    }
    argv[i] = NULL;
    cmd->argv = argv;
    return 1;
}

t_command *build_command(t_token **tokens)
{
    t_command *cmd = malloc(sizeof(t_command));
    t_token *tok = *tokens;

    if (!cmd)
        return NULL;
    cmd->argv = NULL;
    cmd->redirs = NULL;
    cmd->next = NULL;

    if (!fill_command_argv_and_redirs(cmd, &tok))
    {
        free(cmd);
        return NULL;
    }
    *tokens = tok;
    return cmd;
}

t_command *parse_commands(t_token *tokens)
{
    t_command *head = NULL;
    t_command *current = NULL;

    while (tokens)
    {
        t_command *cmd = build_command(&tokens);
        if (!cmd)
        {
            // TODO: ggf. schon erzeugte commands freeen
            return NULL;
        }
        if (!head)
            head = cmd;
        else
            current->next = cmd;
        current = cmd;
        if (tokens && tokens->type == PIPE)
            tokens = tokens->next;
    }
    return head;
}
