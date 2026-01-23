/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 11:12:38 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/23 18:27:41 by kakubo-l         ###   ########.fr       */
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
	/* Do not write a newline here — parent will print/handle the prompt
	   restore to avoid duplicate blank lines on Ctrl+C */
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
