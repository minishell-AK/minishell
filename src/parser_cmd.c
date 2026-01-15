/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 20:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/14 23:56:42 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "../libft/libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int	reject_multiple_out_redirs(t_cmd *cmd);

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

static char	**copy_old_args(char **newargv, char **oldargs, size_t cnt)
{
	memcpy(newargv, oldargs, sizeof(char *) * cnt);
	free(oldargs);
	return (newargv);
}

int	add_arg(t_cmd *cmd, const char *arg)
{
	size_t	cnt;
	char	**newargv;

	if (!cmd)
		return (-1);
	cnt = 0;
	if (cmd->args)
	{
		while (cmd->args[cnt])
			cnt++;
	}
	newargv = malloc(sizeof(char *) * (cnt + 2));
	if (!newargv)
		return (-1);
	if (cmd->args)
		newargv = copy_old_args(newargv, cmd->args, cnt);
	newargv[cnt] = ft_strdup(arg);
	newargv[cnt + 1] = NULL;
	cmd->args = newargv;
	return (0);
}

int	add_redir(t_cmd *cmd, t_redir_type type, const char *target)
{
	t_redir	*r;

	if (!cmd || !target)
		return (-1);
	/* Reject multiple output redirections for the same command */
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
	r->next = cmd->redirs;
	cmd->redirs = r;
	return (0);
}

static int	reject_multiple_out_redirs(t_cmd *cmd)
{
	t_redir *it;

	it = cmd->redirs;
	while (it)
	{
		if (it->type == REDIR_OUT || it->type == APPEND)
		{
			ft_putstr_fd("minishell: syntax error: multiple output redirections\n", STDOUT_FILENO);
			ft_putstr_fd("minishell: syntax error: multiple output redirections\n", 2);
			return (-1);
		}
		it = it->next;
	}
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
