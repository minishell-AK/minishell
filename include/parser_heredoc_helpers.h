/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_helpers.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 01:59:54 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/17 02:00:10 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HEREDOC_HELPERS_H
# define PARSER_HEREDOC_HELPERS_H

# include "parser.h"
# include <signal.h>
# include <sys/types.h>

int		spawn_heredoc_reader(int fd, t_hdoc_ctx *ctx);
int		write_heredoc_entry(int fd, char *line, int expand, char **envp);
char	*read_heredoc_lines(const char *delimiter, int expand, char **envp);
int		install_sigint_ignore(struct sigaction *old_sa);
int		handle_heredoc_wait(pid_t pid, struct sigaction *old_sa);

#endif
