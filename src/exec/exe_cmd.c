/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:04:01 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/20 06:09:59 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_foreground(t_all_variables *all_variables)
{
	if (isatty(STDIN_FILENO)
		&& size_list_cmd(all_variables->cmd) > 0
		&& all_variables->pids[0] > 0)
		tcsetpgrp(STDIN_FILENO, all_variables->pids[0]);
}

static void	restore_foreground(void)
{
	if (isatty(STDIN_FILENO))
		tcsetpgrp(STDIN_FILENO, getpgrp());
}

int	exec_cmd(t_all_variables *all_variables)
{
	int	code;
	int	last_status;

	add_pipe(&all_variables->cmd);
	if (all_variables->cmd && !all_variables->cmd->next
		&& all_variables->cmd->args && all_variables->cmd->args[0]
		&& is_parent_builtin(all_variables->cmd->args[0]))
	{
		code = exec_builtin(all_variables->cmd,
				all_variables->env, all_variables);
		return (code);
	}
	spawn_children(all_variables);
	close_all_pipes(all_variables->cmd);
	handle_foreground(all_variables);
	last_status = waint_all_pids(all_variables->pids,
			size_list_cmd(all_variables->cmd));
	restore_foreground();
	return (last_status);
}
