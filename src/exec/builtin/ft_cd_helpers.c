/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:50:48 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 11:14:21 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value_simple(char **envp, const char *name)
{
	int		i;
	size_t	len;

	if (!envp || !name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	update_pwd_env(char ***envp_ref, const char *old_pwd)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("cd: getcwd error");
		return (1);
	}
	if (old_pwd)
	{
		if (set_env_pair(envp_ref, "OLDPWD", old_pwd) != 0)
			return (1);
	}
	if (set_env_pair(envp_ref, "PWD", cwd) != 0)
		return (1);
	return (0);
}

int	set_env_pair(char ***envp_ref, const char *key, const char *value)
{
	char	*new_entry;
	int		idx;

	new_entry = env_make_entry((char *)key, value);
	if (!new_entry)
		return (1);
	idx = env_find_index(*envp_ref, key);
	if (idx >= 0)
	{
		free((*envp_ref)[idx]);
		(*envp_ref)[idx] = new_entry;
	}
	else
		env_append_entry(envp_ref, new_entry);
	return (0);
}

char	*resolve_cd_path(char **args, char **envp)
{
	char	*path;

	if (!args[1] || ft_strncmp(args[1], "~", 2) == 0)
	{
		path = get_env_value_simple(envp, "HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (NULL);
		}
		return (path);
	}
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env_value_simple(envp, "OLDPWD");
		if (!path)
		{
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
			return (NULL);
		}
		ft_putstr_fd(path, 1);
		ft_putstr_fd("\n", 1);
		return (path);
	}
	return (args[1]);
}

void	obtain_old_pwd(char *current_pwd, char *old_pwd)
{
	if (current_pwd)
		ft_strlcpy(old_pwd, current_pwd, sizeof(old_pwd));
	else if (!getcwd(old_pwd, sizeof(old_pwd)))
		old_pwd[0] = '\0';
}
