/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armeneze <armeneze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 15:44:50 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/06 13:30:09 by armeneze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

int	exec_builtin(t_cmd *cmd, char **env, t_all_variables *all)
{
	(void)all;
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (ft_echo(cmd->args));
	// if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
	// 	return (ft_cd(cmd->args, env));
	if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (ft_pwd(cmd->args));
	if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (ft_env(env, cmd->args));
	// if (ft_strncmp(cmd->args[0], "export", 7) == 0)
	// 	return (ft_export(*env));
	// if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
	// 	return (ft_unset(*env));
	// if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
	// 	return (ft_exit(*env));
	return (0);
}
