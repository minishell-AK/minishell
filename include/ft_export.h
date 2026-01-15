/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created: 2026/01/15 12:55:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 12:55:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_EXPORT_H
# define FT_EXPORT_H

# include "exec.h"

void	print_sorted_env(char **envp);
int	export_variable(char *arg, char ***envp_ref);

#endif
