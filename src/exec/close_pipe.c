/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armeneze <armeneze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 10:33:25 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/02 15:51:09 by armeneze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_all_pipes(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->pipein > 2)
			close(cmd->pipein);
		if (cmd->pipeout > 2)
			close(cmd->pipeout);
		cmd = cmd->next;
	}
}
