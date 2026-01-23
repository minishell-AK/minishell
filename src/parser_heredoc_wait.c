/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_wait.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:01:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/23 18:20:01 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	install_sigint_ignore(struct sigaction *old_sa)
{
	struct sigaction	ign_sa;

	memset(&ign_sa, 0, sizeof(ign_sa));
	ign_sa.sa_handler = SIG_IGN;
	sigemptyset(&ign_sa.sa_mask);
	if (sigaction(SIGINT, &ign_sa, old_sa) == -1)
		return (-1);
	return (0);
}

static int	handle_heredoc_signaled(struct sigaction *old_sa)
{
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	sigaction(SIGINT, old_sa, NULL);
	return (-1);
}

static int	handle_heredoc_exited(int status, struct sigaction *old_sa)
{
	if (WEXITSTATUS(status) == 130)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		sigaction(SIGINT, old_sa, NULL);
		return (-1);
	}
	sigaction(SIGINT, old_sa, NULL);
	return (0);
}

int	handle_heredoc_wait(pid_t pid, struct sigaction *old_sa)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
	{
		sigaction(SIGINT, old_sa, NULL);
		return (-1);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (handle_heredoc_signaled(old_sa));
	if (WIFEXITED(status))
		return (handle_heredoc_exited(status, old_sa));
	sigaction(SIGINT, old_sa, NULL);
	return (-1);
}
