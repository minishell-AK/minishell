/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armeneze <armeneze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 18:17:06 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/05 18:27:00 by armeneze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **env, char **args)
{
	int	i;

	if (count_size_array_char(args) > 1)
	{
		ft_putstr_fd("env: too many arguments\n", 1);
		return (0);
	}
	if (!env)
		return (1);
	i = 0;
	while (env[i])
	{
		ft_putstr_fd(env[i], 1);
		write(1, "\n", 1);
		i++;
	}
	return (0);
}
