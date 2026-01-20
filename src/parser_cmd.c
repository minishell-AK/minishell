/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 20:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/20 10:39:20 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* helper implementations moved to src/parser_cmd_helpers.c */

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
	(void)0;
	return (c);
}

static char	**new_argv_with_arg(char **old, const char *arg)
{
	size_t	cnt;
	char	**newargv;
	char	*tmp;

	cnt = 0;
	if (old)
	{
		while (old[cnt])
			cnt++;
	}
	tmp = ft_strdup(arg);
	if (!tmp)
		return (NULL);
	newargv = malloc(sizeof(char *) * (cnt + 2));
	if (!newargv)
	{
		free(tmp);
		return (NULL);
	}
	if (old)
		memcpy(newargv, old, sizeof(char *) * cnt);
	newargv[cnt] = tmp;
	newargv[cnt + 1] = NULL;
	return (newargv);
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
	r->next = cmd->redirs;
	cmd->redirs = r;
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
