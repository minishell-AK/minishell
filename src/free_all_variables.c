/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all_variables.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armeneze <armeneze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:52:08 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/03 16:36:00 by armeneze         ###   ########.fr       */
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

void	free_all_commands(t_cmd *cmds)
{
	t_cmd	*c;
	t_cmd	*tmp;
	size_t	i;

	c = cmds;
	while (c)
	{
		tmp = c->next;
		if (c->args)
		{
			i = 0;
			while (c->args[i])
			{
				free(c->args[i]);
				i++;
			}
			free(c->args);
		}
		free_redirs(c->redirs);
		free(c);
		c = tmp;
	}
}

void	free_all_variables(t_all_variables *all_variables)
{
	size_t	i;

	if (!all_variables)
		return ;
	if (all_variables->env)
		free(all_variables->env);
	if (all_variables->path)
	{
		i = 0;
		while (all_variables->path[i])
		{
			free(all_variables->path[i]);
			i++;
		}
		free(all_variables->path);
	}
	if (all_variables->pids)
		free(all_variables->pids);
	if (all_variables->cmd)
		free_all_commands(all_variables->cmd);
	free(all_variables);
}
