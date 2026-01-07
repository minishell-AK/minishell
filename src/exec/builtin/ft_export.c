/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 00:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/07 16:12:50 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include <stdio.h>
#include <string.h>

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

static char	*get_var_name(const char *entry)
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

static void	print_sorted_env(char **envp)
{
	int		i;
	int		j;
	char	**sorted;
	char	*tmp;
	int		count;

	count = count_size_array_char(envp);
	sorted = malloc(sizeof(char *) * (count + 1));
	if (!sorted)
		return ;
	i = -1;
	while (++i < count)
		sorted[i] = envp[i];
	sorted[count] = NULL;
	i = -1;
	while (++i < count - 1)
	{
		j = i;
		while (++j < count)
		{
			if (ft_strncmp(sorted[i], sorted[j], ft_strlen(sorted[i])) > 0)
			{
				tmp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = tmp;
			}
		}
	}
	i = -1;
	while (sorted[++i])
	{
		printf("declare -x %s\n", sorted[i]);
	}
	free(sorted);
}

static int	export_variable(char *arg, char ***envp_ref)
{
	char	*name;
	char	*new_entry;
	int		idx;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	if (!ft_strchr(arg, '='))
		return (0);
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
	if (idx >= 0)
	{
		free((*envp_ref)[idx]);
		(*envp_ref)[idx] = new_entry;
	}
	else
		env_append_entry(envp_ref, new_entry);
	return (0);
}

int	ft_export(char **args, char ***envp_ref)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		print_sorted_env(*envp_ref);
		return (0);
	}
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (export_variable(args[i], envp_ref) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
