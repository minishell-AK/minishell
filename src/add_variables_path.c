/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_variables_path.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:55:43 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 10:44:36 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
