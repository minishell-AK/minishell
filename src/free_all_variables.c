/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all_variables.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:52:08 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/15 15:24:39 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static void	free_redirs(t_redir *r)
{
	t_redir	*tmp;

	while (r)
	{
		tmp = r->next;
		if (r->file)
			free(r->file);
		free(r);
		r = tmp;
	}
}

static void	free_string_array(char **arr)
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

static void	free_args(char **args)
{
	size_t i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_all_commands(t_cmd *cmds)
{
	t_cmd	*c;
	t_cmd	*tmp;

	c = cmds;
	while (c)
	{
		tmp = c->next;
		if (c->args)
			free_args(c->args);
		free_redirs(c->redirs);
		free(c);
		c = tmp;
	}
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
		free_all_commands(all_variables->cmd);
	free(all_variables);
}
