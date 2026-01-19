/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:59:32 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 10:48:15 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_assignment(const char *s)
{
	size_t	i;
	char	*eq;

	if (!s || !s[0])
		return (0);
	eq = ft_strchr(s, '=');
	if (!eq)
		return (0);
	if (eq == s)
		return (0);
	if (!ft_isalpha((int)s[0]) && s[0] != '_')
		return (0);
	i = 1;
	while (i < (size_t)(eq - s))
	{
		if (!ft_isalnum((int)s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_extract_name_value(const char *raw, char **name, char **value)
{
	char	*eq;

	eq = ft_strchr(raw, '=');
	if (!eq)
		return (0);
	*name = ft_substr(raw, 0, (size_t)(eq - raw));
	*value = ft_strdup(eq + 1);
	if (!*name || !*value)
	{
		if (*name)
			free(*name);
		if (*value)
			free(*value);
		return (-1);
	}
	return (1);
}

int	assign_env_from_raw(const char *raw, t_cmd **head,
			char	***envp)
{
	char	*name;
	char	*value;

	if (ft_extract_name_value(raw, &name, &value) == -1)
	{
		free_commands(*head);
		return (-1);
	}
	if (!set_env_var(envp, name, value))
	{
		free(name);
		free(value);
		free_commands(*head);
		return (-1);
	}
	free(name);
	free(value);
	return (1);
}

t_token	*handle_assignment(t_token *tk, t_cmd **head, char ***envp)
{
	if (!ft_strchr(tk->raw, '='))
		return (tk->next);
	if (assign_env_from_raw(tk->raw, head, envp) == -1)
		return (NULL);
	return (tk->next);
}
