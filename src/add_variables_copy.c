/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_variables_copy.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:55:03 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 10:44:11 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_partial_string_array(char **arr, int filled)
{
	while (filled-- > 0)
		free(arr[filled]);
	free(arr);
}

static int	get_env_count(char **env)
{
	int	i;

	if (!env)
		return (0);
	i = 0;
	while (env[i] != NULL)
		i++;
	return (i);
}

static int	fill_env_copy(char **env, char **env_copy)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		env_copy[i] = ft_strdup(env[i]);
		if (!env_copy[i])
		{
			free_partial_string_array(env_copy, i);
			return (-1);
		}
		i++;
	}
	return (0);
}

char	**copy_env(char **env)
{
	int		count;
	char	**env_copy;

	if (!env)
		return (NULL);
	count = get_env_count(env);
	env_copy = (char **)malloc((count + 1) * sizeof(char *));
	if (!env_copy)
		return (NULL);
	if (fill_env_copy(env, env_copy) < 0)
		return (NULL);
	env_copy[count] = NULL;
	return (env_copy);
}
