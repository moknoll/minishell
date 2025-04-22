/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:48:17 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/04/22 14:15:32 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_tokens(t_token *list) {
    while (list) {
        printf("Token: %-10s Type: %d\n", list->value, list->type);
        list = list->next;
    }
}

int main(void)
{
	char	*line;
	t_token	*tokens;

	while(1)
	{
		line = ft_readline();
		if (!line) {
			printf("exit\n");
			break;
		}
		if (*line == '\0') {
			free(line);
			continue;
		}
		tokens = tokenizer(line);
		print_tokens(tokens);
		free_tokens(tokens);
		// //expand variables
		// // build commands
		// // execute
		free(line);
	}
	return (0);
}

