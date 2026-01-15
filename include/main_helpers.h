/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created: 2026/01/15 17:00:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 17:00:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HELPERS_H
# define MAIN_HELPERS_H

# include "minishell.h"
# include "exec.h"

int	process_line(char *line, char ***envp_ref, int last_status);
t_all_variables	*prepare_execution(char *line, char ***envp_ref, int last_status);
void	set_locale_c(char ***my_env_ref);
void	setup_terminal_pgrp(void);
int	run_shell_loop(char ***my_env_ref);
void	set_exit_requested(void);
int	get_exit_requested(void);
void	setup_signals(void);

#endif
