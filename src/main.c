/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:58:43 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 10:47:50 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	**my_env;
	int		last_status;

	(void)argc;
	(void)argv;
	setup_signals();
	my_env = dup_envp(envp);
	if (!my_env)
	{
		ft_putstr_fd("minishell: failed to duplicate envp\n", STDERR_FILENO);
		return (1);
	}
	register_envp_ref(&my_env);
	set_locale_c(&my_env);
	setup_terminal_pgrp();
	last_status = run_shell_loop(&my_env);
	rl_clear_history();
	free_envp(my_env);
	unregister_envp_ref();
	return (last_status);
}

/* helper implementations moved to src/main_helpers.c */
