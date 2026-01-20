/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 10:00:00 by kyoshi            #+#    #+#             */
/*   Updated: 2026/01/20 11:26:18 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	build_argv_child(char **argv_child, const char *exe_path,
		const char *template, const char *delimiter)
{
	argv_child[0] = (char *)exe_path;
	argv_child[1] = "--heredoc-child";
	argv_child[2] = (char *)template;
	argv_child[3] = (char *)delimiter;
}

void	set_expand_str(char *expand_str, int expand)
{
	if (expand)
		expand_str[0] = '1';
	else
		expand_str[0] = '0';
	expand_str[1] = '\0';
}

static void	heredoc_sigint(int sig)
{
	(void)sig;
	rl_free_line_state();
	rl_cleanup_after_signal();
	_exit(130);
}

void	heredoc_setup_signals(void)
{
	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 1;
}

void	heredoc_handle_error(int fd, const char *template)
{
	close(fd);
	unlink(template);
	cleanup_and_exit(130);
}
