/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 14:28:56 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/15 02:35:35 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parser.h"
#include "minishell.h"
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>

static void	handle_wait_status(int status, int *last_status, int *seen_sigint);
static void	report_cmd_not_found_and_exit(char *name, t_all_variables *all);
static void	child_prepare_and_exec(t_cmd *cuntent, t_all_variables *all);
static void	set_child_pgrp(int i, t_all_variables *all_variables);
static int	spawn_children(t_all_variables *all_variables);

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
			handle_wait_status(status, &last_status, &seen_sigint);
		i++;
	}
	if (seen_sigint && isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	return (last_status);
}

static void	handle_wait_status(int status, int *last_status, int *seen_sigint)
{
	if (WIFEXITED(status))
		*last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		*last_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			*seen_sigint = 1;
	}
	else
		*last_status = 1;
}

static void	exec_cmd_type(t_cmd *cuntent, char **env, t_all_variables *all)
{
	char	*path;
	int		exit_code;

	/* Guard: ensure command and arguments exist before dereferencing */
	if (!cuntent || !cuntent->args || !cuntent->args[0])
		exit(0);

	if (is_builtin(cuntent->args[0]))
	{
		exit_code = exec_builtin(cuntent, env, all);
		free_all_variables(all);
		exit(exit_code);
	}
	path = find_path(cuntent->args[0], env);
	if (!path)
	{
		report_cmd_not_found_and_exit(cuntent->args[0], all);
	}
	if (execve(path, cuntent->args, env) == -1)
	{
		perror("execve error");
		free(path);
		free_all_variables(all);
		exit(126);
	}
}

static void	report_cmd_not_found_and_exit(char *name, t_all_variables *all)
{
	/* Print error once to stderr (avoid duplicate stdout+stderr lines) */
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
    if (all)
        free_all_variables(all);
    exit(127);
}
static int	spawn_children(t_all_variables *all_variables)
{
	t_cmd	*cuntent;
	int		i;

	cuntent = all_variables->cmd;
	i = 0;
	while (cuntent != NULL)
	{
		all_variables->pids[i] = fork();
		if (all_variables->pids[i] == 0)
		{
			child_prepare_and_exec(cuntent, all_variables);
		}
		else if (all_variables->pids[i] > 0)
		{
			set_child_pgrp(i, all_variables);
		}
		cuntent = cuntent->next;
		i++;
	}
	return (i);
}

static void	child_prepare_and_exec(t_cmd *cuntent, t_all_variables *all)
{
	setpgid(0, 0);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_io(cuntent, all->cmd);
	exec_cmd_type(cuntent, all->env, all);
}

static void	set_child_pgrp(int i, t_all_variables *all_variables)
{
	if (i == 0)
		setpgid(all_variables->pids[i], all_variables->pids[i]);
	else
		setpgid(all_variables->pids[i], all_variables->pids[0]);
}

int	exec_cmd(t_all_variables *all_variables)
{
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
	/* Spawn child processes for each command */
	spawn_children(all_variables);
	close_all_pipes(all_variables->cmd);
	if (isatty(STDIN_FILENO) && size_list_cmd(all_variables->cmd) > 0 && all_variables->pids[0] > 0)
		tcsetpgrp(STDIN_FILENO, all_variables->pids[0]);
	int last_status = waint_all_pids(all_variables->pids, size_list_cmd(all_variables->cmd));
	if (isatty(STDIN_FILENO))
		tcsetpgrp(STDIN_FILENO, getpgrp());
	free_all_variables(all_variables);
	return (last_status);
}
