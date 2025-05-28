/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:54:32 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/28 17:01:06 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* static void ft_cd(char **argv)
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
} */

static int ft_cd(char **argv, t_env **env)
{
    char *path;
    char cwd[PATH_MAX];

    if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
    {
        path = get_variable_value_from_env(*env, "HOME");
        if (!path)
        {
            perror("minishell: cd: HOME not set\n");
            return (1);
        }
    }
    else if (ft_strcmp(argv[1], "-") == 0)
    {
        path = get_variable_value_from_env(*env, "OLDPWD");
        if (!path)
        {
            perror("minishell: cd: OLDPWD not set\n");
            return (1);
        }
    }
    else
    {
        path = argv[1];
    }

    if (chdir(path) == -1)
    {
        printf("minishell: cd: %s: %s\n", path, strerror(errno));
        return (1);
    }

    // Update PWD and OLDPWD
    if (getcwd(cwd, sizeof(cwd)))
    {
        set_env(env, "OLDPWD", get_variable_value_from_env(*env, "PWD"));
        set_env(env, "PWD", cwd);
    }
    return (0);
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

static int ft_pwd(char **argv)
{
	char cwd[1024];

	(void)argv;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
    return (0);
}

static int ft_echo(char **argv)
{
    int i = 1;
    int newline = 1;

    if (argv[1] && ft_strcmp(argv[1], "-n") == 0)
    {
        newline = 0;
        i++;
    }
    while (argv[i])
    {
        printf("%s", argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    return (0);
}

/* void set_env(t_env **env, char *arg)
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
} */

int set_env(t_env **env, const char *key, const char *value)
{
    t_env *tmp = *env;
    t_env *new;

    if (!key || !value)
        return (1); // Error: invalid arguments
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
        {
            free(tmp->value);
            tmp->value = ft_strdup(value);
            tmp->exported = 1;
            return (0); // Success
        }
        tmp = tmp->next;
    }
    // If not found, create new entry
    new = malloc(sizeof(t_env));
    if (!new)
        return (1); // Allocation failed
    new->key = ft_strdup(key);
    new->value = ft_strdup(value);
    new->exported = 1;
    new->next = *env;
    *env = new;
    return (0); // Success
}


/* void ft_env(char **env)
{
	int i;

	i = 0;
	while(env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
} */

int ft_env(char **env)
{
    if (!env || !*env)
    {
        printf("minishell: env: no environment\n");
        return (1);
    }
    while (*env)
    {
        printf("%s\n", *env);
        env++;
    }
    return (0);
}


static int ft_unset(t_env **my_env,const char *key)
{
	t_env *current;
	t_env *previous;

    if (!key || !*key)
    {
        printf("minishell: unset: invalid identifier\n");
        return (1);   
    }
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
    return (0);
}

int ft_unset(t_env **env, const char *key)
{
    t_env *current;
    t_env *prev = NULL;

    if (!key || !*key)
    {
        printf("minishell: unset: invalid identifier\n");
        return (1);
    }

    current = *env;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env = current->next;
            free(current->key);
            free(current->value);
            free(current);
            return (0);
        }
        prev = current;
        current = current->next;
    }
    return (0); // Si la variable n'existe pas, ce n'est pas une erreur
}

/* static void ft_export(t_env *my_env)
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
} */

int ft_export(t_env *env)
{
    if (!env)
    {
        printf("minishell: export: no environment variables\n");
        return (1);
    }
    while (env)
    {
        if (env->exported)
        {
            printf("declare -x %s", env->key);
            if (env->value)
                printf("=\"%s\"", env->value);
            printf("\n");
        }
        env = env->next;
    }
    return (0);
}

/* void builtin(char **argv, t_env **my_env, char **env)
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
} */

void builtin(char **argv, t_env **my_env, char **env)
{
    int status = 0;

    if (!argv[0])
        return;

    if (ft_strcmp(argv[0], "exit") == 0)
        ft_exit(argv);
    else if (ft_strcmp(argv[0], "cd") == 0)
        status = ft_cd(argv, my_env);
    else if (ft_strcmp(argv[0], "pwd") == 0)
        status = ft_pwd(argv);
    else if (ft_strcmp(argv[0], "echo") == 0)
        status = ft_echo(argv);
    else if (ft_strcmp(argv[0], "export") == 0)
    {
        if (!argv[1])
            status = ft_export(*my_env);
        else
        {
            char *eq = ft_strchr(argv[1], '=');
            if (eq)
            {
                *eq = '\0'; // Split key and value
                status = set_env(my_env, argv[1], eq + 1);
            }
            else
                status = set_env(my_env, argv[1], "");
        }
    }
    else if (ft_strcmp(argv[0], "unset") == 0)
        status = ft_unset(my_env, argv[1]);
    else if (ft_strcmp(argv[0], "env") == 0)
        status = ft_env(env);
    else
    {
        printf("minishell: %s: command not found\n", argv[0]);
        status = 127;
    }
    g_exit_status = status;
}
