/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:54:17 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 11:14:21 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	waint_all_pids(int *pids, int size)
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

void	report_cmd_not_found_and_exit(char *name, t_all_variables *all)
{
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	if (all)
		cleanup_child_and_exit(all, 127);
	exit(127);
}

void	cleanup_child_and_exit(t_all_variables *all, int status)
{
	char	***envp_ref;

	envp_ref = NULL;
	if (all)
		envp_ref = all->envp_ref;
	if (all)
		free_all_variables(all);
	if (envp_ref && *envp_ref)
	{
		free_envp(*envp_ref);
		*envp_ref = NULL;
	}
	unregister_envp_ref();
	exit(status);
}
