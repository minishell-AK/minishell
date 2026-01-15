/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created: 2026/01/15 17:45:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 17:45:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_helpers.h"

static volatile sig_atomic_t	g_exit_requested = 0;

void	set_exit_requested(void)
{
	g_exit_requested = 1;
}

int	get_exit_requested(void)
{
	return ((int)g_exit_requested);
}
