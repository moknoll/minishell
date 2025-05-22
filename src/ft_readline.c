/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:34:54 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/22 11:44:01 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_readline(void)
{
	char *line;

	line = readline("minishell$ ");
	if(!line)
	{
		printf("exit\n");
		//herecod Ctrl+D
		exit(0);
	}
	if(*line)
		add_history(line);
	return (line);
}
