/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/15 03:05:38 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Define POSIX feature macros before system headers that need them. */
# define _POSIX_C_SOURCE 200809L

# include <stddef.h>
# include <signal.h>
# include "../libft/libft.h"

/* Global flag set by the signal handler. */
extern volatile sig_atomic_t	g_last_signal;

/* Exit request accessor: prefer functions over a second global variable.
	Use `set_exit_requested()` from other translation units and
	`get_exit_requested()` where needed. */
void	set_exit_requested(void);
int	get_exit_requested(void);

/* utilities provided by the parser/runtime */
char	**dup_envp(char **envp);
char	*env_make_entry(const char *name, const char *value);
void	*xrealloc(void *ptr, size_t new_size);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	free_envp(char **envp);
void	register_envp_ref(char ***envp_ref);
void	unregister_envp_ref(void);
void	cleanup_and_exit(int status);
int		is_space_char(int c);
int		env_append_entry(char ***envp_ref, char *entry);
int		env_find_index(char **envp, const char *name);
int		count_size_array_char(char **array);

/* Signal handler prototypes. */

typedef struct linkedlist
{
	char	**token;
	int		*next;
}	t_list;

int		minishell_loop(void);

#endif
