/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:58:43 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/27 22:54:47 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredoc_child_mode(int argc, char **argv, char **envp)
{
	int		expand;

	if (argc >= 5 && argv[1] && strcmp(argv[1], "--heredoc-child") == 0)
	{
		expand = atoi(argv[4]);
		heredoc_child_exec_main(argv[2], argv[3], expand, envp);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	**my_env;
	int		last_status;

	(void)argc;
	(void)argv;
	if (handle_heredoc_child_mode(argc, argv, envp))
		return (0);
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
	rl_bind_key('\t', (int (*)())rl_insert);
	last_status = run_shell_loop(&my_env);
	rl_clear_history();
	free_envp(my_env);
	register_envp_ref(NULL);
	return (last_status);
}
