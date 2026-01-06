/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armeneze <armeneze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 11:52:19 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/02 14:53:45 by armeneze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "parser.h"
#include "exec.h"
#include "minishell.h"

static void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

static char	*check_path_access(char *dir, char *cmd)
{
	char	*slash_path;
	char	*full_path;

	slash_path = ft_strjoin(ft_strdup(dir), "/");
	if (!slash_path)
		return (NULL);
	full_path = ft_strjoin(slash_path, cmd);
	if (access(full_path, F_OK | X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*find_path(char *cmd, char **env)
{
	char	**paths;
	char	*final_path;
	int		i;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		return (NULL);
	paths = ft_split(env[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		final_path = check_path_access(paths[i], cmd);
		if (final_path)
		{
			free_matrix(paths);
			return (final_path);
		}
		i++;
	}
	free_matrix(paths);
	return (NULL);
}
