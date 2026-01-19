/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:58:50 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 10:48:01 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_old_args(char **newargv, char **oldargs, size_t cnt)
{
	memcpy(newargv, oldargs, sizeof(char *) * cnt);
	free(oldargs);
	return (newargv);
}

int	reject_multiple_out_redirs(t_cmd *cmd)
{
	t_redir		*it;
	const char	*msg;

	it = cmd->redirs;
	msg = "minishell: syntax error: multiple output redirections\n";
	while (it)
	{
		if (it->type == REDIR_OUT || it->type == APPEND)
		{
			ft_putstr_fd(msg, STDOUT_FILENO);
			ft_putstr_fd(msg, 2);
			return (-1);
		}
		it = it->next;
	}
	return (0);
}
