/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslaveev <dslaveev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 13:15:17 by dslaveev          #+#    #+#             */
/*   Updated: 2024/06/26 14:05:58 by dslaveev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

void	init_env(t_env *env, char **environ)
{
	int i;
	char **splited;
	char *key;
	char *value;

	env->vars = NULL;
	env->curr_dir = NULL;
	i = 0;
	if (environ == NULL)
	{
		printf("f thi shi\n");
		return ;
	}
	while (environ[i] != NULL)
	{
		splited = ft_split(environ[i], '=');
		if (splited[0] && splited[1])
		{
			key = splited[0];
			value = splited[1];
			set_env_var(env, key, value);
			if (!strcmp(key, "PWD"))
				env->curr_dir = ft_strdup(value);
		}
		i++;
	}
}

void	set_env_var(t_env *env, const char *key, const char *value)
{
	t_env_var *current;

	current = env->vars;
	while (current != NULL)
	{
		if (!strcmp(current->key, key))
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	t_env_var *new_var = malloc(sizeof(t_env_var));
	if (new_var == NULL)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	new_var->key = ft_strdup(key);
	new_var->value = ft_strdup(value);
	new_var->next = env->vars;
	env->vars = new_var;
}


void	unset_env_var(t_env *env, const char *key)
{
	t_env_var **current;

	current = &env->vars;
	while (*current != NULL)
	{
		if (!strcmp((*current)->key, key))
		{
			t_env_var *delete_env = *current;
			*current = (*current)->next;
			free(delete_env->key);
			free(delete_env->value);
			free(delete_env);
			return ;
		}
		current = &(*current)->next;
	}
}

char	*get_env_var(t_env *env, const char *key)
{
	t_env_var *current;

	current = env->vars;
	while (current != NULL)
	{
		if (!strcmp(current->key, key))
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	change_dir_exec(t_env *env, const char *path)
{
	if (chdir(path) == 0)
	{
		free(env->curr_dir);
		env->curr_dir = getcwd(NULL, 0);
		set_env_var(env, "PWD", env->curr_dir);
	}
	else
	{
		perror("chdir error: ");
	}
}

void	free_env(t_env *env)
{
	t_env_var *current;
	t_env_var *next;

	current = env->vars;
	while (current != NULL)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	free(env->curr_dir);
}
