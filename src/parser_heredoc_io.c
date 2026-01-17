/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_io.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:00:12 by kyoshi            #+#    #+#             */
/*   Updated: 2026/01/16 19:04:45 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	write_heredoc_entry(int fd, char *line, int expand, char **envp)
{
	char	*expanded;

	if (expand)
	{
		expanded = expand_line(line, envp, 0);
		if (!expanded)
			return (0);
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
		free(expanded);
	}
	else
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
	return (1);
}

char	*read_heredoc_lines(const char *delimiter, int expand, char **envp)
{
	char		template[128];
	int			fd;
	int			res;
	t_hdoc_ctx	ctx;

	fd = open_unique_tmpfile(template, sizeof(template));
	if (fd == -1)
		return (NULL);
	ctx.delimiter = delimiter;
	ctx.expand = expand;
	ctx.envp = envp;
	ctx.template = template;
	res = spawn_heredoc_reader(fd, &ctx);
	if (res == -1)
	{
		close(fd);
		unlink(template);
		return (NULL);
	}
	close(fd);
	return (ft_strdup(template));
}
