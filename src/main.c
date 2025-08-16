/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/16 13:31:46 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status = 0;

void	print_error(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (msg)
		ft_putstr_fd(msg, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}

int	is_empty_line(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (1);
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	return (line[i] == '\0');
}

static void	process_command_line(t_data *data)
{
	int		i;
	char	*expanded;

	add_history(data->line);
	data->args = tokenize(data->line);
	if (data->args)
	{
		i = 0;
		while (data->args[i])
		{
			expanded = expand_variables_and_remove_quotes(data->args[i],
					*data->env);
			free(data->args[i]);
			data->args[i] = expanded;
			i++;
		}
		if (data->args[0] && data->args[0][0] == '\0')
		{
			print_error("", "command not found");
			ft_free_tab(data->args);
			g_exit_status = 127;
			return ;
		}
		execute(data);
		ft_free_tab(data->args);
	}
}

void	minishell_loop(t_env **env)
{
	t_data	data;

	data.env = env;
	init_signals_prompt();
	while (1)
	{
		data.line = readline("minishell$ ");
		if (!data.line)
		{
			printf("exit\n");
			break ;
		}
		if (is_empty_line(data.line))
		{
			free(data.line);
			continue ;
		}
		process_command_line(&data);
		free(data.line);
	}
}

int	main(int argc, char *argv[], char *env[])
{
	t_env	*env_list;

	(void)argc, (void)argv;
	env_list = NULL;
	env_list = init_env(&env_list, env);
	if (!env_list)
	{
		print_error(NULL, "failed to initialize environment");
		return (1);
	}
	minishell_loop(&env_list);
	free_env_list(env_list);
	return (g_exit_status);
}
