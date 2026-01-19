/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_alloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 20:26:57 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 10:49:52 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_out(char **out, size_t count)
{
	while (count--)
		free(out[count]);
	free(out);
}

char	**dup_envp(char **envp)
{
	size_t	i;
	size_t	j;
	char	**out;

	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i])
		i++;
	out = malloc(sizeof(char *) * (i + 1));
	if (!out)
		return (NULL);
	j = 0;
	while (j < i)
	{
		out[j] = ft_strdup(envp[j]);
		if (!out[j])
		{
			free_out(out, j);
			return (NULL);
		}
		j++;
	}
	out[j] = NULL;
	return (out);
}

void	free_envp(char **envp)
{
	size_t	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

void	*xrealloc(void *ptr, size_t new_size)
{
	void	*nptr;

	if (!ptr)
		return (malloc(new_size));
	nptr = realloc(ptr, new_size);
	return (nptr);
}
