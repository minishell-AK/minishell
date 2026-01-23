/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 20:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/23 19:09:19 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*cmd_new(void)
{
	t_cmd	*c;

	c = malloc(sizeof(t_cmd));
	if (!c)
		return (NULL);
	c->pipein = STDIN_FILENO;
	c->pipeout = STDOUT_FILENO;
	c->args = NULL;
	c->redirs = NULL;
	c->next = NULL;
	return (c);
}

static void	append_redir_to_list(t_cmd *cmd, t_redir *r)
{
	t_redir	*it;

	if (!cmd->redirs)
	{
		cmd->redirs = r;
		return ;
	}
	it = cmd->redirs;
	while (it->next)
		it = it->next;
	it->next = r;
}

int	add_arg(t_cmd *cmd, const char *arg)
{
	char	**newargv;
	char	**old;

	if (!cmd)
		return (-1);
	newargv = new_argv_with_arg(cmd->args, arg);
	if (!newargv)
		return (-1);
	old = cmd->args;
	cmd->args = newargv;
	if (old)
		free(old);
	return (0);
}

int	add_redir(t_cmd *cmd, t_redir_type type, const char *target)
{
	t_redir	*r;

	if (!cmd || !target)
		return (-1);
	if (type == REDIR_OUT || type == APPEND)
	{
		if (reject_multiple_out_redirs(cmd) == -1)
			return (-1);
	}
	r = malloc(sizeof(t_redir));
	if (!r)
		return (-1);
	r->type = type;
	r->file = ft_strdup(target);
	if (!r->file)
	{
		free(r);
		return (-1);
	}
	r->next = NULL;
	append_redir_to_list(cmd, r);
	return (0);
}

t_cmd	*ensure_cmd(t_cmd **head, t_cmd **cur)
{
	if (!*cur)
	{
		*cur = cmd_new();
		if (!*cur)
			return (NULL);
		if (!*head)
			*head = *cur;
	}
	return (*cur);
}
