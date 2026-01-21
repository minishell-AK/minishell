/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all_variables.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:52:08 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/21 11:09:24 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_string_array(char **arr)
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

void	free_all_variables(t_all_variables *all_variables)
{
	if (!all_variables)
		return ;
	if (all_variables->env)
		free_string_array(all_variables->env);
	if (all_variables->path)
		free_string_array(all_variables->path);
	if (all_variables->pids)
		free(all_variables->pids);
	if (all_variables->cmd)
		free_commands(all_variables->cmd);
	free(all_variables);
}
