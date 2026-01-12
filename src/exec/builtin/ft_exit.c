/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 13:44:07 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/08 18:33:56 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parser.h"
/*
 * New behavior: do not call exit() from here. Instead, signal the main
 * loop that an `exit` was requested so the main loop can perform a
 * single, centralized cleanup and return. This avoids double-free or
 * missed-free situations when ft_exit exits the process abruptly.
 */
#include "exec.h"
#include "parser.h"
#include "minishell.h"

int	ft_exit(t_all_variables *all, char *line)
{
	t_cmd	*cmd;
	(void)line;
	(void)all;
	cmd = all->cmd;
	while (cmd != NULL)
	{
		if (cmd->args && cmd->args[0] && ft_strncmp(cmd->args[0], "exit", 5) == 0)
		{
			/* do not free `line` here; caller (main) owns it and will free it */
			g_exit_requested = 1;
			return (1);
		}
		cmd = cmd->next;
	}
	return (0);
}
