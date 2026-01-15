/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:39:26 by kyoshi            #+#    #+#             */
/*   Updated: 2026/01/14 23:56:42 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "../libft/libft.h"
#include "minishell.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <readline/readline.h>

/* build_tmp_name and try_create_tmp moved to parser_heredoc_tmp.c */

static int	make_tmpfile_loop(char *name, pid_t pid, int *seq,
				char *out, size_t out_sz);

char	*read_trimmed_line(void)
{
	char	*line;
	size_t	len;

	if (isatty(STDIN_FILENO))
	{
		line = readline("heredoc> ");
		return (line);
	}
	line = get_next_line(STDIN_FILENO);
	if (line)
	{
		len = ft_strlen(line);
		if (len && line[len - 1] == '\n')
			line[len - 1] = '\0';
	}
	return (line);
}

// Create project tmp dir if missing and open a unique tmp file
int	open_unique_tmpfile(char *out, size_t out_sz)
{
	char		name[128];
	pid_t		pid;
	static int	seq;

	if (mkdir("tmp", S_IRWXU) == -1 && errno != EEXIST)
		return (-1);
	pid = getpid();
	seq++;
	if (seq <= 0)
		seq = 1;
	return (make_tmpfile_loop(name, pid, &seq, out, out_sz));
}

static int	make_tmpfile_loop(char *name, pid_t pid, int *seq,
				char *out, size_t out_sz)
{
	int fd;

	while (*seq < 100000)
	{
		if (build_tmp_name(name, pid, *seq) == -1)
			return (-1);
		fd = try_create_tmp(name, out, out_sz);
		if (fd != -1)
			return (fd);
		if (errno != EEXIST)
			return (-1);
		(*seq)++;
	}
	return (-1);
}

int	heredoc_read_loop(int fd, t_hdoc_ctx *ctx)
{
	char	*line;
	int		res;

	while (1)
	{
		line = read_trimmed_line();
		if (!line)
		{
			/* readline returned NULL (EOF or error) */
			break ;
		}
		res = process_heredoc_line_ctx(fd, line, ctx);
		if (res == 1)
			return (1);
		if (res == -1)
			return (-1);
	}
	return (0);
}

int	process_heredoc_line_ctx(int fd, char *line, t_hdoc_ctx *ctx)
{
	if (ctx->delimiter && strcmp(line, ctx->delimiter) == 0)
	{
		free(line);
		return (1);
	}
	if (!write_heredoc_entry(fd, line, ctx->expand, ctx->envp))
	{
		free(line);
		close(fd);
		unlink(ctx->template);
		return (-1);
	}
	free(line);
	return (0);
}
