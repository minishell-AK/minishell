/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd_spawn.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copilot <copilot@ai>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:20:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 16:20:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void	handle_wait_status(int status, int *last_status, int *seen_sigint)
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
    char *path;
    int exit_code;

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

int	spawn_children(t_all_variables *all_variables)
{
    t_cmd *cuntent;
    int i;

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
