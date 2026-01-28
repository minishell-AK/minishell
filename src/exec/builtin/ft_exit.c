/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 13:44:07 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/27 21:28:37 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(const char *s)
{
	int		i;

	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	get_exit_status(char **args, int last_status)
{
	long long	code;

	if (!args[1])
		return (last_status);
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		return (255);
	}
	if (args[2] != NULL)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n",
			STDERR_FILENO);
		return (-1);
	}
	code = ft_atoi(args[1]);
	return ((int)(unsigned char)code);
}

int	ft_exit(t_all_variables *all, char *line, int last_status)
{
	t_cmd	*cmd;
	int		status;

	(void)line;
	cmd = all->cmd;
	while (cmd != NULL)
	{
		if (cmd->args && cmd->args[0]
			&& ft_strncmp(cmd->args[0], "exit", 5) == 0)
		{
			status = get_exit_status(cmd->args, last_status);
			if (status == -1)
				return (-1);
			set_exit_requested();
			return (status);
		}
		cmd = cmd->next;
	}
	return (-1);
}
