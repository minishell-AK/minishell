/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 14:28:56 by kakubo-l          #+#    #+#             */
/*   Updated: 2025/12/18 18:12:52 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdio.h>

static const char	*get_redir_name(t_redir_type t)
{
	if (t == REDIR_IN)
		return ("REDIR_IN");
	if (t == REDIR_OUT)
		return ("REDIR_OUT");
	if (t == HEREDOC)
		return ("HEREDOC");
	if (t == APPEND)
		return ("APPEND");
	return ("UNKNOWN");
}

static void	print_args(char **args)
{
	size_t	i;

	if (!args)
	{
		printf("  (no argv)\n");
		return ;
	}
	i = 0;
	while (args[i])
	{
		printf("  argv[%zu] = %s\n", i, args[i]);
		i++;
	}
}

static void	print_redirs(t_redir *r)
{
	const char	*file;

	while (r)
	{
		file = "(null)";
		if (r->file)
			file = r->file;
		printf("  redir type=%s target=%s\n", get_redir_name(r->type), file);
		r = r->next;
	}
}

void	exec_cmd(t_cmd *cmd)
{
	int		idx;
	t_cmd	*c;

	idx = 0;
	c = cmd;
	while (c)
	{
		printf("Command %d:\n", idx);
		print_args(c->args);
		print_redirs(c->redirs);
		c = c->next;
		idx++;
	}
}
