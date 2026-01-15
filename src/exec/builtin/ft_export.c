/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 00:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/15 16:16:50 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "ft_export.h"

int	ft_export(char **args, char ***envp_ref)
{
	int i;
	int ret;

	if (!args[1])
	{
		print_sorted_env(*envp_ref);
		return (0);
	}
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (export_variable(args[i], envp_ref) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}

