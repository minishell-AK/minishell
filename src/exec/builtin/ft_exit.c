/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armeneze <armeneze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 13:44:07 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/06 14:18:01 by armeneze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parser.h"

void	ft_exit(t_all_variables *all, char *line)
{
	t_cmd	*cmd;

	cmd = all->cmd;
	while (cmd != NULL)
	{
		if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		{
			free(line);
			free_all_variables(all);
			exit(0);
		}
		cmd = cmd->next;
	}
}
