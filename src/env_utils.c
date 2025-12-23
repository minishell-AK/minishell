/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 04:30:00 by gitcopilot        #+#    #+#             */
/*   Updated: 2025/12/20 04:26:34 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../libft/libft.h"

char	*env_make_entry(const char *name, const char *value)
{
	char	*tmp;
	char	*entry;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (NULL);
	entry = ft_strjoin(tmp, value);
	free(tmp);
	return (entry);
}

int	env_append_entry(char ***envp_ref, char *entry)
{
	char	**envp;
	size_t	i;

	envp = *envp_ref;
	i = 0;
	while (envp && envp[i])
		i++;
	envp = realloc(envp, sizeof(char *) * (i + 2));
	if (!envp)
	{
		free(entry);
		return (0);
	}
	envp[i] = entry;
	envp[i + 1] = NULL;
	*envp_ref = envp;
	return (1);
}

int	env_find_index(char **envp, const char *name)
{
	size_t	len;
	size_t	i;

	if (!envp || !name)
		return (-1);
	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return ((int)i);
		i++;
	}
	return (-1);
}
