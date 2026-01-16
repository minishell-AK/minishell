/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:58:35 by kyoshi            #+#    #+#             */
/*   Updated: 2026/01/16 19:17:11 by kakubo-l         ###   ########.fr       */
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
