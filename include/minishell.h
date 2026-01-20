/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/20 10:10:21 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L

# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <errno.h>

# include <signal.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <termios.h>

# include <readline/readline.h>
# include <readline/history.h>

# include "../libft/libft.h"
# include "add_variables.h"
# include "cleanup_state.h"
# include "exec.h"
# include "expander_core.h"
# include "ft_cd_helpers.h"
# include "ft_export.h"
# include "lexer.h"
# include "lexer_word_finalize_helpers.h"
# include "main_helpers.h"
# include "parser.h"
# include "parser_cmd_helpers.h"
# include "parser_helpers.h"
# include "parser_heredoc_helpers.h"

extern volatile sig_atomic_t	g_last_signal;

void	set_exit_requested(void);
int		get_exit_requested(void);

char	**dup_envp(char **envp);
char	*env_make_entry(const char *name, const char *value);
void	*xrealloc(void *ptr, size_t new_size);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	free_envp(char **envp);
void	register_envp_ref(char ***envp_ref);
void	unregister_envp_ref(void);
void	cleanup_and_exit(int status);
/* instrumentation removed for production build */
int		is_space_char(int c);
int		env_append_entry(char ***envp_ref, char *entry);
int		env_find_index(char **envp, const char *name);
int		count_size_array_char(char **array);

typedef struct linkedlist
{
	char	**token;
	int		*next;
}	t_list;

int		minishell_loop(void);

#endif
