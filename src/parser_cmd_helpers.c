/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:58:50 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/23 19:10:31 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_old_args(char **newargv, char **oldargs, size_t cnt)
{
	memcpy(newargv, oldargs, sizeof(char *) * cnt);
	free(oldargs);
	return (newargv);
}

int	reject_multiple_out_redirs(t_cmd *cmd)
{
	(void)cmd;
	return (0);
}

char	**new_argv_with_arg(char **old, const char *arg)
{
	size_t		cnt;
	char		**newargv;
	char		*tmp;

	cnt = 0;
	if (old)
	{
		while (old[cnt])
			cnt++;
	}
	tmp = ft_strdup(arg);
	if (!tmp)
		return (NULL);
	newargv = malloc(sizeof(char *) * (cnt + 2));
	if (!newargv)
	{
		free(tmp);
		return (NULL);
	}
	if (old)
		memcpy(newargv, old, sizeof(char *) * cnt);
	newargv[cnt] = tmp;
	newargv[cnt + 1] = NULL;
	return (newargv);
}
