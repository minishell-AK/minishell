#include "exec.h"
#include "parser.h"
#include "minishell.h"
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>

int	exec_cmd(t_all_variables *all_variables)
{
	add_pipe(&all_variables->cmd);
	if (all_variables->cmd && !all_variables->cmd->next
		&& all_variables->cmd->args && all_variables->cmd->args[0]
		&& is_parent_builtin(all_variables->cmd->args[0]))
		{
			int code = exec_builtin(
				all_variables->cmd, all_variables->env, all_variables);
			free_all_variables(all_variables);
			return (code);
		}
	spawn_children(all_variables);
	close_all_pipes(all_variables->cmd);
	if (isatty(STDIN_FILENO)
		&& size_list_cmd(all_variables->cmd) > 0
		&& all_variables->pids[0] > 0)
		tcsetpgrp(STDIN_FILENO, all_variables->pids[0]);
	int last_status = waint_all_pids(
		all_variables->pids, size_list_cmd(all_variables->cmd));
	if (isatty(STDIN_FILENO))
		tcsetpgrp(STDIN_FILENO, getpgrp());
	free_all_variables(all_variables);
	return (last_status);
}
