/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 00:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/16 17:55:57 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
