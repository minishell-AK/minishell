/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armeneze <armeneze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 13:56:45 by armeneze          #+#    #+#             */
/*   Updated: 2025/12/16 13:28:46 by armeneze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_fd(char *filename, int type)
{
	int	fd;

	if (type == REDIR_IN)
		fd = open(filename, O_RDONLY);
	else if (type == REDIR_OUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	return (fd);
}
