/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_set.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:52:09 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 10:42:34 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha((unsigned char)str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum((unsigned char)str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static	char	*get_var_name(const char *entry)
{
	int		i;
	char	*name;

	i = 0;
	while (entry[i] && entry[i] != '=')
		i++;
	name = malloc(i + 1);
	if (!name)
		return (NULL);
	ft_strlcpy(name, entry, i + 1);
	return (name);
}

static void	assign_or_append(int idx, char ***envp_ref, char *new_entry)
{
	if (idx >= 0)
	{
		free((*envp_ref)[idx]);
		(*envp_ref)[idx] = new_entry;
	}
	else
		env_append_entry(envp_ref, new_entry);
}

static int	set_env_entry(char *arg, char ***envp_ref)
{
	char	*name;
	char	*new_entry;
	int		idx;

	name = get_var_name(arg);
	if (!name)
		return (1);
	new_entry = ft_strdup(arg);
	if (!new_entry)
	{
		free(name);
		return (1);
	}
	idx = env_find_index(*envp_ref, name);
	free(name);
	assign_or_append(idx, envp_ref, new_entry);
	return (0);
}

int	export_variable(char *arg, char ***envp_ref)
{
	if (!(!arg) && !is_valid_identifier(arg))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	if (!ft_strchr(arg, '='))
		return (0);
	return (set_env_entry(arg, envp_ref));
}
