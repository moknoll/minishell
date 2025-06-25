/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copilot <copilot@ai>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 00:00:00 by copilot          #+#    #+#             */
/*   Updated: 2025/06/25 00:00:00 by copilot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void handle_heredocs(t_command *cmd_list)
{
    setup_heredoc_signals();
    t_command *cmd = cmd_list;

    while (cmd)
    {
        t_redir *r = cmd->redirs;
        while (r)
        {
            if (r->type == REDIR_HEREDOC)
            {
                int pipefd[2];
                char *line;
                if (pipe(pipefd) == -1)
                {
                    perror("minishell: heredoc pipe");
                    exit(1);
                }
                while (1)
                {
                    line = readline("heredoc> ");
                    if (!line)
                        break;
                    if (ft_strcmp(line, r->file) == 0)
                    {
                        free(line);
                        break;
                    }
                    write(pipefd[1], line, ft_strlen(line));
                    write(pipefd[1], "\n", 1);
                    free(line);
                }
                close(pipefd[1]);
                r->fd = pipefd[0];
            }
            r = r->next;
        }
        cmd = cmd->next;
    }
    setup_parent_signals();
}
