/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 10:19:16 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/16 18:15:15 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_redirections(t_redir **redir_ptr, int *fd_in, int *fd_out);

static int	process_in_redir(t_redir *redir, int fd_in)
{
	int	newfd;

	if (fd_in > STDERR_FILENO)
		close(fd_in);
	if (redir->type == HEREDOC)
	{
		newfd = open(redir->file, O_RDONLY);
		if (newfd == -1)
		{
			perror(redir->file);
			return (-1);
		}
		unlink(redir->file);
	}
	else
	{
		newfd = get_fd(redir->file, redir->type);
		if (newfd == -1)
			return (-1);
	}
	return (newfd);
}

static int	process_out_redir(t_redir *redir, int fd_out)
{
	int	newfd;

	if (fd_out > STDERR_FILENO)
		close(fd_out);
	newfd = get_fd(redir->file, redir->type);
	if (newfd == -1)
		return (-1);
	return (newfd);
}

void	setup_child_io(t_cmd *cmd, t_cmd *head_list)
{
	t_redir	*redir;
	int		fd_in;
	int		fd_out;

	fd_in = cmd->pipein;
	fd_out = cmd->pipeout;
	redir = cmd->redirs;
	process_redirections(&redir, &fd_in, &fd_out);
	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (fd_out != STDOUT_FILENO)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	close_all_pipes(head_list);
}

static void	process_redirections(t_redir **redir_ptr, int *fd_in, int *fd_out)
{
	t_redir	*r;

	r = *redir_ptr;
	while (r != NULL)
	{
		if (r->type == REDIR_IN || r->type == HEREDOC)
		{
			*fd_in = process_in_redir(r, *fd_in);
			if (*fd_in == -1)
				exit(1);
		}
		else if (r->type == REDIR_OUT || r->type == APPEND)
		{
			*fd_out = process_out_redir(r, *fd_out);
			if (*fd_out == -1)
				exit(1);
		}
		r = r->next;
	}
}
