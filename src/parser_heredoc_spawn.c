/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_spawn.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:00:23 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/27 22:43:12 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

static void	exec_heredoc_child(const char *template, t_hdoc_ctx *ctx)
{
	char	exe_path[PATH_MAX + 1];
	char	*argv_child[6];
	char	expand_str[2];
	ssize_t	rlen;

	rlen = readlink("/proc/self/exe", exe_path, PATH_MAX);
	if (rlen <= 0)
	{
		exe_path[0] = '\0';
		ft_strlcpy(exe_path, "/proc/self/exe", PATH_MAX + 1);
	}
	else
		exe_path[rlen] = '\0';
	build_argv_child(argv_child, exe_path, template, ctx->delimiter);
	set_expand_str(expand_str, ctx->expand);
	argv_child[4] = expand_str;
	argv_child[5] = NULL;
	execve(exe_path, argv_child, ctx->envp);
	exit(127);
}

static int	install_sigquit_ignore(struct sigaction *old_quit_sa)
{
	struct sigaction	ign_quit;

	memset(&ign_quit, 0, sizeof(ign_quit));
	ign_quit.sa_handler = SIG_IGN;
	sigemptyset(&ign_quit.sa_mask);
	ign_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &ign_quit, old_quit_sa) == -1)
		return (-1);
	return (0);
}

static pid_t	spawn_heredoc_fork(const char *template, t_hdoc_ctx *ctx,
						struct sigaction *old_sa,
						struct sigaction *old_quit_sa)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		sigaction(SIGINT, old_sa, NULL);
		sigaction(SIGQUIT, old_quit_sa, NULL);
		return (-1);
	}
	if (pid == 0)
		exec_heredoc_child(template, ctx);
	return (pid);
}

int	spawn_heredoc_reader(const char *template, t_hdoc_ctx *ctx)
{
	pid_t				pid;
	int					status;
	struct sigaction	old_sa;
	struct sigaction	old_quit_sa;

	if (install_sigint_ignore(&old_sa) == -1)
		return (-1);
	if (install_sigquit_ignore(&old_quit_sa) == -1)
	{
		sigaction(SIGINT, &old_sa, NULL);
		return (-1);
	}
	pid = spawn_heredoc_fork(template, ctx, &old_sa, &old_quit_sa);
	if (pid == -1)
		return (-1);
	status = handle_heredoc_wait(pid, &old_sa);
	sigaction(SIGQUIT, &old_quit_sa, NULL);
	return (status);
}

int	heredoc_child_exec_main(const char *template, const char *delimiter,
	signed int expand, char **envp)
{
	int			fd;
	t_hdoc_ctx	ctx;
	int			rc;

	fd = open(template, O_CREAT | O_TRUNC | O_WRONLY | O_CLOEXEC,
			S_IRUSR | S_IWUSR);
	if (fd == -1)
		exit(1);
	register_heredoc(fd, template);
	heredoc_setup_signals();
	ctx.delimiter = delimiter;
	ctx.expand = expand;
	ctx.envp = envp;
	ctx.template = (char *)template;
	rc = heredoc_read_loop(fd, &ctx);
	if (rc == -1)
		heredoc_handle_error(fd, template);
	close(fd);
	register_heredoc(-1, NULL);
	exit(0);
	return (0);
}
