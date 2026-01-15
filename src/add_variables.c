/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 14:13:40 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/15 16:16:50 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "exec.h"
#include "minishell.h"
#include "add_variables.h"
#include <stdlib.h>



t_all_variables	*add_variables(t_cmd *cmd, char ***envp_ref)
{
	t_all_variables	*vars;

	vars = (t_all_variables *)malloc(sizeof(t_all_variables));
	if (!vars)
		return (NULL);
	vars->cmd = cmd;
	vars->envp_ref = envp_ref;
	vars->env = copy_env(*envp_ref);
	if (!vars->env && *envp_ref)
	{
		free(vars);
		return (NULL);
	}
	vars->path = create_array_path(*envp_ref);
	vars->pids = (int *)malloc((size_list_cmd(cmd)) * sizeof(int));
	if (!vars->pids)
	{
		free_vars_and_arrays(vars);
		return (NULL);
	}
	return (vars);
}
