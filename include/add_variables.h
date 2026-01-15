/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_variables.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created: 2026/01/15 12:45:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 12:45:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADD_VARIABLES_H
# define ADD_VARIABLES_H

# include "exec.h"

char	**copy_env(char **env);
char	**create_array_path(char **env);
void	free_vars_and_arrays(t_all_variables *vars);

#endif
