/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:55:56 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/27 22:54:47 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <dirent.h>
#include <limits.h>

static t_cleanup_state	*cleanup_state(void)
{
	static t_cleanup_state	st = {NULL, 0, -1, NULL};

	return (&st);
}

static void	g_registered_cleanup(void)
{
	t_cleanup_state	*st;

	st = cleanup_state();
	clear_history();
	if (st->envp_ref && *(st->envp_ref))
	{
		free_envp(*(st->envp_ref));
		*(st->envp_ref) = NULL;
	}
}

void	register_envp_ref(char ***envp_ref)
{
	t_cleanup_state	*st;

	st = cleanup_state();
	if (!envp_ref)
	{
		st->envp_ref = NULL;
		return ;
	}
	st->envp_ref = envp_ref;
	if (!st->atexit_registered)
	{
		atexit(g_registered_cleanup);
		st->atexit_registered = 1;
	}
}

void	register_heredoc(int fd, const char *template)
{
	t_cleanup_state	*st;

	st = cleanup_state();
	st->heredoc_fd = fd;
	st->heredoc_template = (char *)template;
}

void	cleanup_and_exit(int status)
{
	t_cleanup_state	*st;

	st = cleanup_state();
	if (st->envp_ref && *(st->envp_ref))
	{
		free_envp(*(st->envp_ref));
		*(st->envp_ref) = NULL;
	}
	clear_history();
	if (st->heredoc_fd > STDERR_FILENO)
		close(st->heredoc_fd);
	if (st->heredoc_template)
		unlink(st->heredoc_template);
	st->envp_ref = NULL;
	exit(status);
}
