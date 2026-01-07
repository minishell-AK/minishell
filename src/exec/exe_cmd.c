/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 14:28:56 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/07 15:45:27 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parser.h"
#include "minishell.h"

static void	waint_all_pids(int *pids, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
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

void	exec_cmd(t_all_variables *all_variables)
{
	t_cmd	*cuntent;
	int		i;

	add_pipe(&all_variables->cmd);
	/* Se é comando único (sem pipes) e é builtin do pai, executa no pai */
	if (all_variables->cmd && !all_variables->cmd->next
		&& all_variables->cmd->args && all_variables->cmd->args[0]
		&& is_parent_builtin(all_variables->cmd->args[0]))
	{
		exec_builtin(all_variables->cmd, all_variables->env, all_variables);
		free_all_variables(all_variables);
		return ;
	}
	cuntent = all_variables->cmd;
	i = 0;
	while (cuntent != NULL)
	{
		all_variables->pids[i] = fork();
		if (all_variables->pids[i] == 0)
		{
			setup_child_io(cuntent, all_variables->cmd);
			exec_cmd_type(cuntent, all_variables->env, all_variables);
		}
		cuntent = cuntent->next;
		i++;
	}
	close_all_pipes(all_variables->cmd);
	waint_all_pids(all_variables->pids, size_list_cmd(all_variables->cmd));
	free_all_variables(all_variables);
}
