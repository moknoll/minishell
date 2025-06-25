/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:29:50 by radubos           #+#    #+#             */
/*   Updated: 2025/05/24 17:29:55 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int apply_redirections(t_redir *redirs)
{
    int fd;
    int std_fd;

    while (redirs)
    {
        if (redirs->type == REDIR_IN)
        {
            fd = open(redirs->file, O_RDONLY);
            std_fd = STDIN_FILENO;
        }
        else if (redirs->type == REDIR_OUT)
        {
            fd = open(redirs->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            std_fd = STDOUT_FILENO;
        }
        else if (redirs->type == REDIR_APPEND)
        {
            fd = open(redirs->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
            std_fd = STDOUT_FILENO;
        }
        else if (redirs->type == REDIR_HEREDOC)
        {
            if (redirs->fd == -1)
            {
                printf("minishell: heredoc pipe not set\n");
                return (-1);
            }
            fd = redirs->fd;
            std_fd = STDIN_FILENO;
        }
        else
        {
            printf("minishell: unknown redirection type\n");
            return (-1);
        }

        if (fd < 0)
        {
            printf("minishell: %s: %s\n", redirs->file, strerror(errno));
            return (-1);
        }

        if (dup2(fd, std_fd) == -1)
        {
            printf("minishell: dup2 failed\n");
            close(fd);
            return (-1);
        }

        close(fd);
        redirs = redirs->next;
    }
    return (0);
}