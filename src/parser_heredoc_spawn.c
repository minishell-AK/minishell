/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_spawn.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:00:23 by kyoshi            #+#    #+#             */
/*   Updated: 2026/01/16 19:03:43 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_sigint(int sig)
{
	(void)sig;
	rl_free_line_state();
	rl_cleanup_after_signal();
	_exit(130);
}

static void	heredoc_child_process(int fd, t_hdoc_ctx *ctx)
{
	int	rc;

	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 1;
	rc = heredoc_read_loop(fd, ctx);
	if (rc == -1)
		_exit(130);
	_exit(0);
}

int	spawn_heredoc_reader(int fd, t_hdoc_ctx *ctx)
{
	pid_t				pid;
	int					status;
	struct sigaction	old_sa;

	if (install_sigint_ignore(&old_sa) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		sigaction(SIGINT, &old_sa, NULL);
		return (-1);
	}
	if (pid == 0)
	{
		heredoc_child_process(fd, ctx);
	}
	status = handle_heredoc_wait(pid, &old_sa);
	return (status);
}
