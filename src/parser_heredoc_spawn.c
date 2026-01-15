/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_spawn.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created: 2026/01/15 18:40:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 18:40:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "parser_heredoc_helpers.h"
#include "minishell.h"
#include "../libft/libft.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

static void	heredoc_sigint(int sig)
{
	(void)sig;
	rl_free_line_state();
	rl_cleanup_after_signal();
	_exit(130);
}

static void	heredoc_child_process(int fd, t_hdoc_ctx *ctx)
{
	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 1;
	int rc = heredoc_read_loop(fd, ctx);
	if (rc == -1)
		_exit(130);
	_exit(0);
}

int	spawn_heredoc_reader(int fd, t_hdoc_ctx *ctx)
{
	pid_t pid;
	int status;
	struct sigaction old_sa;

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
