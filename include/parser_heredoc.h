/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 02:18:12 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 10:40:35 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HEREDOC_H
# define PARSER_HEREDOC_H

# include <sys/types.h>
# include <stddef.h>

int		build_tmp_name(char *name, pid_t pid, int seq);
int		try_create_tmp(const char *name, char *out, size_t out_sz);
char	*read_trimmed_line(void);

#endif
