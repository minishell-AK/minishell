/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armeneze <armeneze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 14:13:40 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/05 15:55:49 by armeneze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "exec.h"
#include "minishell.h"
#include <stdlib.h>

char	*find_path_string(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

char	**copy_env(char **env)
{
	int		i;
	char	**env_copy;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i] != NULL)
		i++;
	env_copy = (char **)malloc((i + 1) * sizeof(char *));
	if (!env_copy)
		return (NULL);
	i = 0;
	while (env[i] != NULL)
	{
		env_copy[i] = env[i];
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}

char	**create_array_path(char **env)
{
	char	*path_start;
	char	*path_copy_to_free;
	char	**array_env;

	path_start = find_path_string(env);
	if (!path_start)
		return (NULL);
	path_copy_to_free = ft_strdup(path_start);
	if (!path_copy_to_free)
		return (NULL);
	array_env = ft_split(path_copy_to_free, ':');
	free(path_copy_to_free);
	return (array_env);
}

t_all_variables	*add_variables(t_cmd *cmd, char **env)
{
	t_all_variables	*vars;

	vars = (t_all_variables *)malloc(sizeof(t_all_variables));
	if (!vars)
		return (NULL);
	vars->cmd = cmd;
	vars->env = copy_env(env);
	if (!vars->env && env)
	{
		free(vars);
		return (NULL);
	}
	vars->path = create_array_path(env);
	vars->pids = (int *)malloc((size_list_cmd(cmd)) * sizeof(int));
	return (vars);
}
