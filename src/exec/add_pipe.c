/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armeneze <armeneze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 09:18:48 by armeneze          #+#    #+#             */
/*   Updated: 2025/12/01 11:24:10 by armeneze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_pipe(t_cmd **cmd_list)
{
	t_cmd	*current;
	int		pipefd[2];

	current = *cmd_list;
	while (current->next != NULL)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe error");
			return ;
		}
		current->pipeout = pipefd[1];
		current->next->pipein = pipefd[0];
		current = current->next;
	}
}
