/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_helpers.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 02:48:44 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/17 02:48:57 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CD_HELPERS_H
# define FT_CD_HELPERS_H

# include "exec.h"

char	*get_env_value_simple(char **envp, const char *name);
int		update_pwd_env(char ***envp_ref, const char *old_pwd);
int		set_env_pair(char ***envp_ref, const char *key, const char *value);
char	*resolve_cd_path(char **args, char **envp);
void	obtain_old_pwd(char *current_pwd, char *old_pwd);

#endif
