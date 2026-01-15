/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 23:50:37 by kyoshi            #+#    #+#             */
/*   Updated: 2026/01/14 21:18:47 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "../libft/libft.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <readline/readline.h>
#include <termios.h>

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

static void heredoc_sigint(int sig)
{
	(void)sig;
	rl_free_line_state();
	rl_cleanup_after_signal();
	_exit(130);
}

static int	spawn_heredoc_reader(int fd, t_hdoc_ctx *ctx)
{
	pid_t	pid;
	int	status;
	struct sigaction old_sa;
	struct sigaction ign_sa;

	/* Temporarily ignore SIGINT in parent so only the heredoc child handles it */
	memset(&ign_sa, 0, sizeof(ign_sa));
	ign_sa.sa_handler = SIG_IGN;
	sigemptyset(&ign_sa.sa_mask);
	sigaction(SIGINT, &ign_sa, &old_sa);

	pid = fork();
	if (pid == -1)
	{
		/* restore previous handler on error */
		sigaction(SIGINT, &old_sa, NULL);
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, heredoc_sigint);
		signal(SIGQUIT, SIG_IGN);
		/* Let readline handle terminal signals in the heredoc child */
		rl_catch_signals = 1;
		int rc = heredoc_read_loop(fd, ctx);
		if (rc == -1)
			_exit(130);
		_exit(0);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		sigaction(SIGINT, &old_sa, NULL);
		return (-1);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		/* Ensure a newline separates heredoc output from prompt, then restore readline state */
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		/* restore original SIGINT handler before returning */
		sigaction(SIGINT, &old_sa, NULL);
		return (-1);
	}
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 130)
		{
			/* Child exited with code 130 (interrupted) */
			/* Ensure a newline separates heredoc output from prompt, then restore readline state */
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			/* restore original SIGINT handler before returning */
			sigaction(SIGINT, &old_sa, NULL);
			return (-1);
		}
		return (0);
	}
	sigaction(SIGINT, &old_sa, NULL);
	return (-1);
}

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
