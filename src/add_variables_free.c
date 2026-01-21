/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_variables_free.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:55:10 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/21 11:08:16 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_string_array(char **arr)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_vars_and_arrays(t_all_variables *vars)
{
	if (!vars)
		return ;
	if (vars->cmd)
		free_commands(vars->cmd);
	if (vars->path)
		free_string_array(vars->path);
	if (vars->env)
		free_string_array(vars->env);
	free(vars);
}
