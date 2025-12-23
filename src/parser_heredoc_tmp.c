/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_tmp.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 01:20:00 by kyoshi            #+#    #+#             */
/*   Updated: 2025/12/21 21:48:44 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "../libft/libft.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int	build_tmp_name(char *name, pid_t pid, int seq)
{
	char	*tmp;

	tmp = ft_itoa((int)pid);
	if (!tmp)
		return (-1);
	ft_strlcpy(name, "tmp/minishell_heredoc_", 128);
	ft_strlcat(name, tmp, 128);
	free(tmp);
	tmp = ft_itoa(seq);
	if (!tmp)
		return (-1);
	ft_strlcat(name, "_", 128);
	ft_strlcat(name, tmp, 128);
	free(tmp);
	return (0);
}

int	try_create_tmp(const char *name, char *out, size_t out_sz)
{
	int	fd;

	fd = open(name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1)
		return (-1);
	if (ft_strlen(name) + 1 > out_sz)
	{
		close(fd);
		unlink(name);
		return (-1);
	}
	ft_strlcpy(out, name, out_sz);
	return (fd);
}
