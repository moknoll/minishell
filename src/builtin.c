/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:54:32 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/22 11:24:56 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while(s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

static void ft_cd(char **argv)
{
	char *path;

	if (argv[1])
		path = argv[1];
	else
		path = getenv("HOME");
	if (!path)
	{
		perror("cd: HOME not set\n");
		return;
	}
	if (chdir(path) == -1)
		perror("cd");
}

static void ft_exit(char **argv)
{
	if (argv[1])
		perror("exit: too many arguments\n");
	else
	{
		printf("exit\n");
		exit(0);
	}
}

static void ft_pwd(char **argv)
{
	char cwd[1024];

	(void)argv;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
}

static void ft_echo(char **argv)
{

	int i = 1;

	if (!argv[1])
	{
		printf("\n");
		return ;
	}
	while(argv[i])
	{
		printf("%s", argv[i]);
		if(argv[i+1])
			printf(" ");
		i++;
	}
	printf("\n");
	}

static int check_multiple_n(char *str)
{
	char valid;
	int i;

	i = 0;
	valid = 'n';
	while (str[i] && ft_strcmp(str, "-n"))
	{
		if(str[i] == '-')
			i++;
		if(str[i] != valid)
			return (0);
		i++;
	}
	return (1);
}
static void ft_echo_n(char **argv)
{
	int i = 2;
	{
		while(argv[i])
		{
			printf("%s", argv[i]);
			if(argv[i+1])
				printf(" ");
			i++;
		}
	}
}

void set_env(t_env **env, char *arg)
{
    int sep = 0;
    t_env *tmp = *env;

    while (arg[sep] && arg[sep] != '=')
        sep++;

    if (sep == 0 || !arg[sep])
        return; // Ungültig: Kein Key oder kein '='

    char *key = ft_strndup(arg, sep);
    char *value = ft_strdup(arg + sep + 1);

    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
        {
            free(tmp->value);
            tmp->value = value;
            tmp->exported = 1;
            free(key);
            return;
        }
        tmp = tmp->next;
    }

    // Falls nicht existiert → neuen Eintrag anhängen
    t_env *new = malloc(sizeof(t_env));
    new->key = key;
    new->value = value;
    new->exported = 1;
    new->next = *env;
    *env = new;
}


void ft_env(char **env)
{
	int i;

	i = 0;
	while(env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}


static void ft_unset(t_env **my_env, char *key)
{
	t_env *current;
	t_env *previous;

	current = *my_env;
	previous = NULL;
	while(current)
	{
		if(ft_strcmp(current->key, key) == 0)
		{
			if(previous == NULL)
				*my_env = current->next;
			else
				previous->next = current->next;
			return(free(current->value), free(current->key), free(current));
		}
		previous = current;
		current = current ->next;
	}
	printf("unset done");
}

static void ft_export(t_env *my_env)
{
	while (my_env)
	{
		if(my_env->exported)
		{
			printf("declare -x %s", my_env->key);
			if(my_env->value)
				printf("=\"%s\"", my_env->value);
			printf("\n");
		}
		my_env = my_env->next;
	}
}

void builtin(char **argv, t_env **my_env, char **env)
{
	if (!argv[0] || argv[0][0] == '\0')
		return;
	if (ft_strcmp(argv[0], "exit") == 0)
		ft_exit(argv);
	else if (ft_strcmp(argv[0], "cd") == 0)
		ft_cd(argv);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		ft_pwd(argv);
	else if (ft_strcmp(argv[0], "echo") == 0)
	{
		if (argv[1] && check_multiple_n(argv[1]))
			ft_echo_n(argv);
		else
			ft_echo(argv);
	}
	else if (ft_strcmp(argv[0], "export") == 0)
	{
		if (!argv[1])
			ft_export(*my_env);
		else
			set_env(my_env, argv[1]);
	}
	else if (ft_strcmp(argv[0], "unset") == 0)
	 	ft_unset(my_env, argv[1]);
	else if (ft_strcmp(argv[0], "env") == 0)
		ft_env(env);
	else
		perror("command not found");
}
