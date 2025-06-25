/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:34:54 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/25 20:03:59 by radubos          ###   ########.fr       */
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
		exit(0);
	}
	if(*line)
		add_history(line);
	return (line);
}
