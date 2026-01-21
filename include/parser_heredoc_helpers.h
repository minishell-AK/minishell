/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_helpers.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 01:59:54 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/20 21:14:50 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HEREDOC_HELPERS_H
# define PARSER_HEREDOC_HELPERS_H

# include "parser.h"
# include <signal.h>
# include <sys/types.h>

int		spawn_heredoc_reader(const char *template, t_hdoc_ctx *ctx);
int		heredoc_child_exec_main(const char *template, const char *delimiter,
			int expand, char **envp);
void	build_argv_child(char **argv_child, const char *exe_path,
			const char *template, const char *delimiter);
void	set_expand_str(char *expand_str, int expand);
void	heredoc_setup_signals(void);
void	heredoc_handle_error(int fd, const char *template);
int		write_heredoc_entry(int fd, char *line, int expand, char **envp);
char	*read_heredoc_lines(const char *delimiter, int expand, char **envp);
int		install_sigint_ignore(struct sigaction *old_sa);
int		handle_heredoc_wait(pid_t pid, struct sigaction *old_sa);

#endif
