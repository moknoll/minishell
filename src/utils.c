/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:31:08 by radubos           #+#    #+#             */
/*   Updated: 2025/06/25 23:01:34 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// not implemented yet
char	*ft_malloc(char *src, int size)
{
	src = malloc(sizeof(char) * size);
	if (!src)
		return (exit(1), NULL);
	return (src);
}

// not implemented yet
char	*ft_realloc(char *src, int old_size, int new_size)
{
	char	*res;
	int		i;

	res = NULL;
	i = 0;
	if (!src)
		return (NULL);
	if (new_size < 0)
		return (src);
	ft_malloc(res, new_size);
	if (!res)
		return (NULL);
	while (old_size--)
	{
		res[i] = src[i];
		i++;
	}
	res[i] = '\0';
	free(src);
	return (res);
}
