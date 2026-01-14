/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 10:19:16 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/13 19:44:41 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parser.h"
#include "minishell.h"

void	setup_child_io(t_cmd *cmd, t_cmd *head_list)
{
	t_redir	*redir;
	int		fd_in;
	int		fd_out;

	fd_in = cmd->pipein;
	fd_out = cmd->pipeout;
	redir = cmd->redirs;
	while (redir != NULL)
	{
		if (redir->type == REDIR_IN)
		{
			if (fd_in > STDERR_FILENO)
				close(fd_in);
			fd_in = get_fd(redir->file, redir->type);
			if (fd_in == -1)
				exit(1);
		}
		else if (redir->type == HEREDOC)
		{
			if (fd_in > STDERR_FILENO)
				close(fd_in);
			fd_in = open(redir->file, O_RDONLY);
			if (fd_in == -1)
			{
				perror(redir->file);
				exit(1);
			}
			unlink(redir->file);
		}
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			if (fd_out > STDERR_FILENO)
				close(fd_out);
			fd_out = get_fd(redir->file, redir->type);
			if (fd_out == -1)
				exit(1);
		}
		redir = redir->next;
	}
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
