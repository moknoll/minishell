/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:30:06 by radubos           #+#    #+#             */
/*   Updated: 2025/06/27 00:09:44 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>  // pour rl_replace_line, rl_on_new_line, rl_redisplay

static int	apply_redirections(t_redir *redirs)
{
	int	fd;
	int	std_fd;

	while (redirs)
	{
		if (open_redir_file(redirs, &fd, &std_fd) == -1)
			return (-1);
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

void	child_process(t_command *cmd, int prev_fd, int pipefd[2], char **env)
{
	setup_child_signals();
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	if (apply_redirections(cmd->redirs) == -1)
	{
		perror("minishell: redirection");
		exit(1);
	}
	if (cmd->argv[0][0] == '/' || cmd->argv[0][0] == '.')
		execve(cmd->argv[0], cmd->argv, env);
	else
	{
		char *path = get_path(cmd->argv[0], env);
		if (path)
		{
			execve(path, cmd->argv, env);
			free(path);
		}
	}
	perror("minishell");
	exit(127);
}

static void	wait_for_last(pid_t last_pid, int *status)
{
	if (last_pid != -1)
	{
		waitpid(last_pid, status, 0);
		if (WIFEXITED(*status))
			g_exit_status = WEXITSTATUS(*status);
		else
			g_exit_status = 1;
	}
}

static pid_t	launch_child_and_manage_pipes(t_command *cmd, int *prev_fd,
														int *pipefd, char **env)
{
	pid_t	pid;

	if (cmd->next && pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		exit(1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		exit(1);
	}
	else if (pid == 0)
		child_process(cmd, *prev_fd, pipefd, env);
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
	else
		*prev_fd = -1;
	return (pid);
}

void	execute_external(t_command *cmd_list, char **env)
{
    int		pipefd[2];
    int		prev_fd;
    pid_t	pid;
    int		status;
    int		last_pid;

    /* 1) Le parent ignore SIGINT/SIGQUIT pendant l’exécution */
    ignore_signals();

    /* 2) On utilise les handlers heredoc uniquement pour handle_heredocs */
    setup_heredoc_signals();
    handle_heredocs(cmd_list);
    restore_heredoc_signals();

    prev_fd = -1;
    last_pid = -1;
    while (cmd_list)
    {
        pid = launch_child_and_manage_pipes(cmd_list, &prev_fd, pipefd, env);
        last_pid = pid;
        cmd_list = cmd_list->next;
    }

    /* 3) On attend le dernier enfant et on ferme le dernier pipe */
    wait_for_last(last_pid, &status);
    if (prev_fd != -1)
        close(prev_fd);

    /* si le dernier fils a été tué par SIGINT (Ctrl-C), on affiche une ligne vide */
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        write(STDOUT_FILENO, "\n", 1);

    /* 4) Si l’enfant est mort sur SIGQUIT, on affiche "Quit (core dumped)" */
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
        write(STDOUT_FILENO, "Quit (core dumped)\n", 19);

    /* 5) On remet nos handlers SIGINT/SIGQUIT pour le prompt */
    setup_parent_signals();

    /* 6) On met à jour g_exit_status en fonction du signal ou du code */
    if (WIFSIGNALED(status))
        g_exit_status = 128 + WTERMSIG(status);
    else
        g_exit_status = WEXITSTATUS(status);
}