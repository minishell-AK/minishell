/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:00:50 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 10:48:58 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	make_tmpfile_loop(pid_t pid, int *seq, char *out, size_t out_sz);

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
	pid_t		pid;
	static int	seq;

	if (mkdir("tmp", S_IRWXU) == -1 && errno != EEXIST)
		return (-1);
	pid = getpid();
	seq++;
	if (seq <= 0)
		seq = 1;
	return (make_tmpfile_loop(pid, &seq, out, out_sz));
}

static int	make_tmpfile_loop(pid_t pid, int *seq, char *out, size_t out_sz)
{
	int		fd;
	char	name[128];

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
