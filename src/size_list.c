/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   size_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armeneze <armeneze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:52:41 by armeneze          #+#    #+#             */
/*   Updated: 2025/12/01 16:54:45 by armeneze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_size_array_char(char **array)
{
	int	i;

	if (!array)
		return (0);
	i = 0;
	while (array[i])
		i++;
	return (i);
}

int	size_list_cmd(t_cmd *cmd)
{
	int		i;
	t_cmd	*cuntent;

	i = 0;
	cuntent = cmd;
	while (cuntent != NULL)
	{
		i++;
		cuntent = cuntent->next;
	}
	return (i);
}
