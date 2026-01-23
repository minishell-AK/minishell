/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 11:52:19 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/23 13:17:35 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

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

static int	is_regular_file(const char *path)
{
	struct stat	buf;

	if (stat(path, &buf) != 0)
		return (0);
	return (S_ISREG(buf.st_mode));
}

static char	*check_path_access(char *dir, char *cmd)
{
	char	*slash_path;
	char	*full_path;

	slash_path = ft_strjoin(dir, "/");
	if (!slash_path)
		return (NULL);
	full_path = ft_strjoin(slash_path, cmd);
	free(slash_path);
	if (!full_path)
		return (NULL);
	if (access(full_path, F_OK | X_OK) == 0 && is_regular_file(full_path))
		return (full_path);
	free(full_path);
	return (NULL);
}

static char	*get_and_search_paths(char **env, char *cmd)
{
	int		i;
	char	**paths;
	char	*final_path;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		return (NULL);
	paths = ft_split(env[i] + 5, ':');
	if (!paths)
		return (NULL);
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

char	*find_path(char *cmd, char **env)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0 && is_regular_file(cmd))
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (get_and_search_paths(env, cmd));
}
