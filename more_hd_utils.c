/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_hd_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/04 12:53:56 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_realloc(void *ptr, size_t new_size)
{
	void	*new_ptr;

	new_ptr = malloc(new_size);
	if (ptr == NULL)
		return (malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, new_size);
	free(ptr);
	return (new_ptr);
}

int	ft_getchar(void)
{
	char	c;
	ssize_t	result;

	result = read(STDIN_FILENO, &c, 1);

	if (result == 1)
		return ((unsigned char)c);
	else
		return (-1);
}
