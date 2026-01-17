/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 00:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/16 17:37:09 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	shift_env_left(char **dst, char **src, int start, int count);

static int	is_valid_var_name(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha((unsigned char)str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum((unsigned char)str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	remove_env_entry(char ***envp_ref, int idx)
{
	int		i;
	int		count;
	char	**new_envp;

	count = count_size_array_char(*envp_ref);
	new_envp = malloc(sizeof(char *) * count);
	if (!new_envp)
		return ;
	i = 0;
	while (i < idx)
	{
		new_envp[i] = (*envp_ref)[i];
		i++;
	}
	free((*envp_ref)[idx]);
	shift_env_left(new_envp, *envp_ref, i, count);
	free(*envp_ref);
	*envp_ref = new_envp;
}

static void	shift_env_left(char **dst, char **src, int start, int count)
{
	int	i;

	i = start;
	while (i < count - 1)
	{
		dst[i] = src[i + 1];
		i++;
	}
	dst[i] = NULL;
}

static int	unset_variable(char *var_name, char ***envp_ref)
{
	int	idx;

	if (!is_valid_var_name(var_name))
	{
		ft_putstr_fd("unset: `", 2);
		ft_putstr_fd(var_name, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	idx = env_find_index(*envp_ref, var_name);
	if (idx >= 0)
		remove_env_entry(envp_ref, idx);
	return (0);
}

int	ft_unset(char **args, char ***envp_ref)
{
	int	i;
	int	ret;

	if (!args[1])
		return (0);
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (unset_variable(args[i], envp_ref) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
