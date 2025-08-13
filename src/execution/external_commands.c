/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritz <moritz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:38:39 by mknoll            #+#    #+#             */
/*   Updated: 2025/08/12 15:09:40 by moritz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*prepare_command_path(char **args, t_env *env)
{
	char	*path;

	if (!args || !args[0])
		return (NULL);
	if (ft_strchr(args[0], '/'))
		path = ft_strdup(args[0]);
	else
		path = find_command_path(args[0], env);
	return (path);
}

void	execute_child_process(char **args, char *path, t_env *env)
{
	char	**env_array;

	reset_signals_to_default();
	if (!process_redirections(args))
		exit(1);
	env_array = env_to_array(env);
	if (execve(path, args, env_array) == -1)
	{
		print_error(args[0], "No such file or directory");
		ft_free_tab(env_array);
		free(path);
		exit(127);
	}
}

void	handle_parent_process(pid_t pid, char *path, int *status)
{
	ignore_sigint();
	waitpid(pid, status, 0);
	free(path);
	if (WIFEXITED(*status))
		g_exit_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		g_exit_status = 128 + WTERMSIG(*status);
	handle_status_and_print(*status);
	init_signals_prompt();
}

void	launch_extern_command_simple(char **args, t_env *env)
{
	pid_t	pid;
	char	*path;
	int		status;

	path = prepare_command_path(args, env);
	if (!path)
	{
		print_error(args[0], "command not found");
		g_exit_status = 127;
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(path);
		g_exit_status = 1;
		return ;
	}
	if (pid == 0)
		execute_child_process(args, path, env);
	else
		handle_parent_process(pid, path, &status);
}
