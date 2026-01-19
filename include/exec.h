/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 15:40:20 by armeneze          #+#    #+#             */
/*   Updated: 2026/01/19 11:14:21 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "parser.h"
# include "minishell.h"
# include "../libft/libft.h"

typedef struct s_all_variables
{
	char	**env;
	char	***envp_ref;
	char	**path;
	int		*pids;
	t_cmd	*cmd;
}	t_all_variables;

t_all_variables	*add_variables(t_cmd *cmd, char ***envp_ref);
int				waint_all_pids(int *pids, int size);
int				spawn_children(t_all_variables *all_variables);
void			report_cmd_not_found_and_exit(char *name, t_all_variables *all);
void			handle_wait_status(int status,
					int	*last_status,	int	*seen_sigint);
int				exec_cmd(t_all_variables *all_variables);
void			add_pipe(t_cmd **cmd);
int				open_in(char *file);
int				open_out(char *file);
void			cleanup_child_and_exit(t_all_variables *all, int status);
void			close_file(int fd);
int				size_list_cmd(t_cmd *cmd);
void			close_all_pipes(t_cmd *cmd);
void			setup_child_io(t_cmd *cmd, t_cmd *head_list);
int				get_fd(char *filename, int type);
void			free_all_variables(t_all_variables *all_variables);
char			*find_path(char *cmd, char **env);
int				is_builtin(char *cmd);
int				is_parent_builtin(char *cmd);
int				exec_builtin(t_cmd *cmd, char **env, t_all_variables *all);
int				ft_echo(char **args);
int				ft_cd(char **args, char ***envp_ref);
int				ft_export(char **args, char ***envp_ref);
int				ft_unset(char **args, char ***envp_ref);
int				ft_pwd(char **args);
int				ft_env(char **env, char **args);
int				ft_exit(t_all_variables *all, char *line, int last_status);

#endif
