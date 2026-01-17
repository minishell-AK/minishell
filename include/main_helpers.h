/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 02:32:26 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/17 02:47:12 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HELPERS_H
# define MAIN_HELPERS_H

# include "minishell.h"
# include "exec.h"

int				process_line(char *line, char ***envp_ref, int last_status);
t_all_variables	*prepare_execution(char *line,
					char ***envp_ref,
					int last_status);
void			set_locale_c(char ***my_env_ref);
void			setup_terminal_pgrp(void);
int				run_shell_loop(char ***my_env_ref);
void			set_exit_requested(void);
int				get_exit_requested(void);
void			setup_signals(void);

#endif
