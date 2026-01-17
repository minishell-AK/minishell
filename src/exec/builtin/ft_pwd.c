/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armeneze <armeneze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 17:48:59 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/06 13:57:06 by armeneze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(char **args)
{
	char	cwd[PATH_MAX];

	if (count_size_array_char(args) > 1)
	{
		ft_putstr_fd("pwd: too many arguments\n", 1);
		return (0);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr_fd(cwd, 1);
		write(1, "\n", 1);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}
