/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:26:56 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/05 14:53:18 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_strlen(const char *str)
{
	int	len;

	len = 0;
	while (str[len])
	{
		len++;
	}
	return (len);
}

static void	ft_strcat(char *dest, char *src)
{
	unsigned int	i;
	unsigned int	j;

	j = 0;
	i = 0;
	while (dest[i])
	{
		i++;
	}
	while (src[j])
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len_s1;
	int		len_s2;
	char	*new_string;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	new_string = (char *)malloc(len_s1 + len_s2 + 1);
	if (!new_string)
		return (NULL);
	new_string[0] = '\0';
	ft_strcat(new_string, (char *)s1);
	ft_strcat(new_string, (char *)s2);
	return (new_string);
}

static int count_size(int n)
{
	int count = 0;

	if(n < 0)
		n*= -1;
	while(n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char *ft_itoa(int num)
{
	char *dst;
	int count;
	int i;
	long int n;

	n = num;
	count = count_size(n);
	i = 0;
	if (n < 0 || count == 0)
		count++;
	if(!(dst = (char * )malloc((count+1) * sizeof(char))))
	if(n < 0)
	{
		n *= -1;
		dst[0] = '-';
		i++;
	}
	while(count > i)
	{
		count--;
		dst[count] = (n % 10) + '0';
		n /= 10;
		}
	return (dst);
}

int	ft_isalnum(int c)
{
	if ((c >= '0' && c <= '9')
		|| ((c >= 65 && c <= 90)
			|| (c >= 97 && c <= 122)))
		return (1);
	else
		return (0);
}
