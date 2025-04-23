/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:48:17 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/04/23 09:46:22 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_tokens(t_token *list) {
    while (list) {
        printf("Token: %-10s Type: %d\n", list->value, list->type);
        list = list->next;
    }
}

void free_args(char **argv)
{
	int i ;

	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

int main(void)
{
	char	*line;
	t_token	*tokens;
	char **args;

	init_signal();
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

		// //expand variables
		args = token_to_argv(tokens);
		for (int j = 0; args[j]; j++)
    		printf("argv[%d]: [%s]\n", j, args[j]);
		builtin(args);
		// // execute
		free_tokens(tokens);
		free(line);
		free_args(args);
	}
	return (0);
}

