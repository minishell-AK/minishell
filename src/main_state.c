/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:58:35 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 10:47:40 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t	*exit_requested_slot(void)
{
	static volatile sig_atomic_t	flag = 0;

	return (&flag);
}

void	set_exit_requested(void)
{
	*exit_requested_slot() = 1;
}

int	get_exit_requested(void)
{
	return ((int)(*exit_requested_slot()));
}
