/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:58:43 by kyoshi            #+#    #+#             */
/*   Updated: 2026/01/15 19:58:44 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"
#include "parser.h"
#include "exec.h"
#include "lexer.h"
#include "../libft/libft.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "main_helpers.h"


int	main(int argc, char **argv, char **envp)
{
	char	**my_env;
	int	last_status;

	(void)argc;
	(void)argv;
	setup_signals();
	my_env = dup_envp(envp);
	if (!my_env)
	{
		fprintf(stderr, "minishell: failed to duplicate envp\n");
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
