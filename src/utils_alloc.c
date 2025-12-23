/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_alloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 20:26:57 by kyoshi            #+#    #+#             */
/*   Updated: 2025/12/20 03:50:00 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <string.h>

#include "../libft/libft.h"

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
	nptr = malloc(new_size);
	if (!nptr)
		return (NULL);
	memcpy(nptr, ptr, new_size);
	free(ptr);
	return (nptr);
}
