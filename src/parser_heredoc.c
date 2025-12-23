/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 23:50:37 by kyoshi            #+#    #+#             */
/*   Updated: 2025/12/20 02:00:55 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "../libft/libft.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <readline/readline.h>

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
/* helpers provided by src/parser_heredoc_utils.c and parser_heredoc_tmp.c */

static char	*read_heredoc_lines(const char *delimiter, int expand,
	char **envp)
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
	res = heredoc_read_loop(fd, &ctx);
	if (res == -1)
	{
		close(fd);
		return (NULL);
	}
	close(fd);
	return (ft_strdup(template));
}

char	*create_heredoc(t_token *delim_token, char **envp)
{
	int		expand;
	char	*tmpfile;
	t_seg	*s;

	expand = 1;
	if (delim_token->segs)
	{
		s = delim_token->segs;
		while (s)
		{
			if (s->type == SEG_SINGLE_QUOTED || s->type == SEG_DOUBLE_QUOTED)
			{
				expand = 0;
				break ;
			}
			s = s->next;
		}
	}
	tmpfile = read_heredoc_lines(delim_token->raw, expand, envp);
	return (tmpfile);
}
