/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:55:56 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/20 08:47:11 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cleanup_state	*cleanup_state(void)
{
	static t_cleanup_state	st = {NULL, 0};

	return (&st);
}

static void	g_registered_cleanup(void)
{
	t_cleanup_state	*st;

	st = cleanup_state();
	clear_history();
	if (st->envp_ref && *(st->envp_ref))
	{
		free_envp(*(st->envp_ref));
		*(st->envp_ref) = NULL;
	}
}

void	register_envp_ref(char ***envp_ref)
{
	t_cleanup_state	*st;

	if (!envp_ref)
		return ;
	st = cleanup_state();
	st->envp_ref = envp_ref;
	if (!st->atexit_registered)
	{
		atexit(g_registered_cleanup);
		st->atexit_registered = 1;
	}
}

void	unregister_envp_ref(void)
{
	cleanup_state()->envp_ref = NULL;
}

void	cleanup_and_exit(int status)
{
	t_cleanup_state	*st;

	st = cleanup_state();
	if (st->envp_ref && *(st->envp_ref))
	{
		free_envp(*(st->envp_ref));
		*(st->envp_ref) = NULL;
	}
	clear_history();
	st->envp_ref = NULL;
	exit(status);
}
