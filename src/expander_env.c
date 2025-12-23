/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 18:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2025/12/20 04:26:34 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "../libft/libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>

char	*get_env_value(char **envp, const char *name)
{
	size_t	len;
	size_t	i;

	len = strlen(name);
	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

/* removed int_to_str to reduce function count; use ft_itoa directly */

int	expand_status(t_exp *ctx, size_t *i)
{
	char	*code;

	code = ft_itoa(ctx->last_status);
	if (!code)
		return (0);
	if (!expand_buf(ctx, code))
	{
		free(code);
		return (0);
	}
	free(code);
	*i += 2;
	return (1);
}

/* helpers moved to src/env_utils.c to satisfy Norminette */

int	set_env_var(char ***envp_ref, const char *name, const char *value)
{
	char	**envp;
	int		idx;
	char	*entry;

	if (!envp_ref || !name)
		return (0);
	envp = *envp_ref;
	idx = env_find_index(envp, name);
	if (idx >= 0)
	{
		entry = env_make_entry(name, value);
		if (!entry)
			return (0);
		free(envp[idx]);
		envp[idx] = entry;
		return (1);
	}
	entry = env_make_entry(name, value);
	if (!entry)
		return (0);
	return (env_append_entry(envp_ref, entry));
}
