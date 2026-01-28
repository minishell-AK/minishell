/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:05:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/27 21:41:24 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

static void	free_redirs(t_redir *r)
{
	t_redir	*tmp;

	while (r)
	{
		tmp = r->next;
		if (r->file)
		{
			free(r->file);
		}
		free(r);
		r = tmp;
	}
}

static void	free_args(char **args)
{
	size_t	i;

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

void	free_commands(t_cmd *cmds)
{
	t_cmd	*c;
	t_cmd	*tmp;

	c = cmds;
	while (c)
	{
		tmp = c->next;
		if (c->pipein > 2)
			close(c->pipein);
		if (c->pipeout > 2)
			close(c->pipeout);
		if (c->args)
		{
			free_args(c->args);
		}
		free_redirs(c->redirs);
		free(c);
		c = tmp;
	}
}
