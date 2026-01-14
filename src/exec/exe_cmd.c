/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 14:28:56 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/13 20:21:00 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parser.h"
#include "minishell.h"
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>

static int	waint_all_pids(int *pids, int size)
{
	int	i;
	int	status;
	int	last_status;
	int	seen_sigint;

	i = 0;
	last_status = 0;
	seen_sigint = 0;
	while (i < size)
	{
		if (waitpid(pids[i], &status, 0) > 0)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				last_status = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGINT)
					seen_sigint = 1;
			}
			else
				last_status = 1;
		}
		i++;
	}
	if (seen_sigint && isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	return (last_status);
}

static void	exec_cmd_type(t_cmd *cuntent, char **env, t_all_variables *all)
{
	char	*path;
	int		exit_code;

	if (is_builtin(cuntent->args[0]))
	{
		exit_code = exec_builtin(cuntent, env, all);
		exit(exit_code);
	}
	if (!cuntent->args || !cuntent->args[0])
		exit(0);
	path = find_path(cuntent->args[0], env);
	if (!path)
	{
		ft_putstr_fd(cuntent->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	if (execve(path, cuntent->args, env) == -1)
	{
		perror("execve error");
		free(path);
		exit(126);
	}
}

int	exec_cmd(t_all_variables *all_variables)
{
	t_cmd	*cuntent;
	int		i;

	add_pipe(&all_variables->cmd);
	/* Se é comando único (sem pipes) e é builtin do pai, executa no pai */
	if (all_variables->cmd && !all_variables->cmd->next
		&& all_variables->cmd->args && all_variables->cmd->args[0]
		&& is_parent_builtin(all_variables->cmd->args[0]))
	{
		int code = exec_builtin(all_variables->cmd, all_variables->env, all_variables);
		free_all_variables(all_variables);
		return (code);
	}
	cuntent = all_variables->cmd;
	i = 0;
	while (cuntent != NULL)
	{
		all_variables->pids[i] = fork();
		if (all_variables->pids[i] == 0)
		{
			setpgid(0, 0);
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			setup_child_io(cuntent, all_variables->cmd);
			exec_cmd_type(cuntent, all_variables->env, all_variables);
		}
		else if (all_variables->pids[i] > 0)
		{
			if (i == 0)
				setpgid(all_variables->pids[i], all_variables->pids[i]);
			else
				setpgid(all_variables->pids[i], all_variables->pids[0]);
		}
		cuntent = cuntent->next;
		i++;
	}
	close_all_pipes(all_variables->cmd);
	if (isatty(STDIN_FILENO) && size_list_cmd(all_variables->cmd) > 0 && all_variables->pids[0] > 0)
		tcsetpgrp(STDIN_FILENO, all_variables->pids[0]);
	int last_status = waint_all_pids(all_variables->pids, size_list_cmd(all_variables->cmd));
	if (isatty(STDIN_FILENO))
		tcsetpgrp(STDIN_FILENO, getpgrp());
	free_all_variables(all_variables);
	return (last_status);
}
