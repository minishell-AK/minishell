/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_helpers.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created: 2026/01/15 16:40:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 16:40:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CD_HELPERS_H
# define FT_CD_HELPERS_H

# include "exec.h"

char	*get_env_value_simple(char **envp, const char *name);
int	update_pwd_env(char ***envp_ref, const char *old_pwd);
int	set_env_pair(char ***envp_ref, const char *key, const char *value);
char	*resolve_cd_path(char **args, char **envp);
void	obtain_old_pwd(char *current_pwd, char *old_pwd);

#endif
