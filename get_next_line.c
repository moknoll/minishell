#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

char *ft_realloc(char *s, int old_size, int new_size)
{
	if (old_size >= new_size)
		return (s);
	char *res = malloc(sizeof(char) * new_size);
	int i = 0;
	while (old_size--)
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	free(s);
	return (res);
}

char *get_next_line(int fd)
{
	static char buffer[BUFFER_SIZE];
	static int buffer_read = 0;
	static int buffer_index = 0;
	char *res = malloc(sizeof(char));
	int res_size = 1;
	int i = 0;
	while (1)
	{
		if (buffer_index == 0)
		{
			buffer_read = read(fd, buffer, BUFFER_SIZE);
			if (buffer_read == -1)
			{
				free(res);
				res = NULL;
				return (NULL);
			}
			if (buffer_read == 0)
			{
				if (i == 0)
				{
					free(res);
					return (NULL);
				}
				break;
			}
		}
		res = ft_realloc(res, res_size, res_size + buffer_read);
		if (!res)
			return (NULL);
		res_size += buffer_read;
		while (buffer_index < buffer_read && buffer[buffer_index] != '\n')
			res[i++] = buffer[buffer_index++];
		if (buffer[buffer_index] == '\n')
		{
			res[i++] = buffer[buffer_index++];
			if (i == buffer_read)
			{
				buffer_index = 0;
			}
			break;
		}
		buffer_index = 0;
	}
	res[i] = 0;
	return (res);
}
