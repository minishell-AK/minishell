/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_variables_free.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created: 2026/01/15 18:10:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 18:10:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "add_variables.h"
#include <stdlib.h>

void	free_string_array(char **arr)
{
	size_t i;

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
	if (vars->path)
		free_string_array(vars->path);
	if (vars->env)
		free_string_array(vars->env);
	free(vars);
}
