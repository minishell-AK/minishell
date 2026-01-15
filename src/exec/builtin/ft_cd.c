/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 00:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/14 23:56:42 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

static int	set_env_pair(char ***envp_ref, const char *key, const char *value);
static void	obtain_old_pwd(char *current_pwd, char *old_pwd);

static char	*get_env_value_simple(char **envp, const char *name)
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

static int	update_pwd_env(char ***envp_ref, const char *old_pwd)
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

static int	set_env_pair(char ***envp_ref, const char *key, const char *value)
{
	char *new_entry;
	int idx;

	new_entry = env_make_entry((char *)key, value);
	if (!new_entry)
		return (1);
	idx = env_find_index(*envp_ref, key);
	if (idx >= 0)
		(*envp_ref)[idx] = new_entry;
	else
		env_append_entry(envp_ref, new_entry);
	return (0);
}

static char	*resolve_cd_path(char **args, char **envp)
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

int	ft_cd(char **args, char ***envp_ref)
{
	char	*path;
	char	old_pwd[PATH_MAX];
	char	*current_pwd;

	if (args[1] && args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	current_pwd = get_env_value_simple(*envp_ref, "PWD");
	obtain_old_pwd(current_pwd, old_pwd);
	path = resolve_cd_path(args, *envp_ref);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		perror(path);
		return (1);
	}
	return (update_pwd_env(envp_ref, old_pwd));
}

static void	obtain_old_pwd(char *current_pwd, char *old_pwd)
{
	if (current_pwd)
		ft_strlcpy(old_pwd, current_pwd, sizeof(old_pwd));
	else if (!getcwd(old_pwd, sizeof(old_pwd)))
		old_pwd[0] = '\0';
}
