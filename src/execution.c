/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:30:06 by radubos           #+#    #+#             */
/*   Updated: 2025/06/25 19:03:11 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_external(t_command *cmd_list, char **env)
{
    // Preprocess heredocs: prompt user and fill pipes
    handle_heredocs(cmd_list);
    int pipefd[2];
    int prev_fd = -1;
    pid_t pid;
    int status;
    int last_pid = -1;

    while (cmd_list)
    {
        // Create pipe only if there's a next command
        if (cmd_list->next && pipe(pipefd) == -1)
        {
            perror("minishell: pipe");
            return;
        }
        pid = fork();
        if (pid < 0)
        {
            perror("minishell: fork");
            return;
        }
        else if (pid == 0) // Child process
        {
            // Reset signals for child
            setup_child_signals();
            // Handle input from previous command
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            // Handle output to next command
            if (cmd_list->next)
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
            // Apply redirections (check for failure)
            if (apply_redirections(cmd_list->redirs) == -1)
            {
                perror("minishell: redirection");
                exit(1);
            }
            // Execute command
            if (cmd_list->argv[0][0] == '/' || cmd_list->argv[0][0] == '.')
            {
                execve(cmd_list->argv[0], cmd_list->argv, env);
            }
            else
            {
                execvp(cmd_list->argv[0], cmd_list->argv);
            }
            // If we get here, exec failed
            perror("minishell");
            exit(127); // Command not found
        }
        // Parent process
        // Close previous pipe input (if any)
        if (prev_fd != -1)
            close(prev_fd);
        // Save write end of pipe for next command
        if (cmd_list->next)
        {
            close(pipefd[1]);
            prev_fd = pipefd[0];
        }
        last_pid = pid; // Save last child PID
        cmd_list = cmd_list->next;
    }

    // Wait for last command to finish
    if (last_pid != -1)
    {
        waitpid(last_pid, &status, 0);
        if (WIFEXITED(status))
            g_exit_status = WEXITSTATUS(status);
        else
            g_exit_status = 1;
    }
    // Clean up any remaining pipe ends
    if (prev_fd != -1)
        close(prev_fd);
}
