/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:05:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/06 18:43:21 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_commands(t_cmd *cmds)
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
