/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/15 03:05:38 by kyoshi           ###   ########.fr       */
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
/* lexer/parser inspection removed; restore normal prompt behavior */

volatile sig_atomic_t	g_last_signal = 0;
static volatile sig_atomic_t	g_exit_requested = 0;

static t_all_variables	*prepare_execution(char *line, char ***envp_ref, int last_status);
static void		set_locale_c(char ***my_env_ref);
static void		setup_terminal_pgrp(void);
static int		run_shell_loop(char ***my_env_ref);

/* Accessors for exit request flag. Make the flag static to avoid
   exposing additional globals; other translation units should call
   `set_exit_requested()` to request exit. */
void	set_exit_requested(void)
{
	g_exit_requested = 1;
}

int	get_exit_requested(void)
{
	return ((int)g_exit_requested);
}

void	sigint_handler(int sig)
{
	(void)sig;
	g_last_signal = sig;
	/* print ^C so user sees the control char, then newline */
	write(1, "^C\n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigquit_handler(int sig)
{
	(void)sig;
}

static void	setup_signals(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		perror("sigaction(SIGINT)");
	sa.sa_handler = sigquit_handler;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		perror("sigaction(SIGQUIT)");
	/* Ignore job-control signals in the interactive shell to avoid
	   being suspended when manipulating terminal foreground. */
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);

	/* Let our handler manage SIGINT instead of readline's internals. */
	rl_catch_signals = 0;
}

static int	process_line(char *line, char ***envp_ref, int last_status)
{
	t_all_variables *all;

	add_history(line);
	all = prepare_execution(line, envp_ref, last_status);
	if (!all)
		return (last_status);
	{
		int exit_status = ft_exit(all, line, last_status);
		if (exit_status >= 0)
		{
			free_all_variables(all);
			return (exit_status);
		}
	}
	last_status = exec_cmd(all);
	return (last_status);
}

static t_all_variables	*prepare_execution(char *line, char ***envp_ref,
					int last_status)
{
	t_token *tokens;
	t_cmd *cmd;
	t_all_variables *all;

	tokens = lexer_tokenize(line);
	if (!tokens)
		return (NULL);
	expand_tokens(tokens, *envp_ref, last_status);
	cmd = parse_tokens(tokens, envp_ref);
	token_free_all(tokens);
	if (!cmd)
		return (NULL);
	all = add_variables(cmd, envp_ref);
	if (!all)
	{
		fprintf(stderr, "minishell: failed to prepare execution\n");
		free_commands(cmd);
	}
	return (all);
}

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
	/* register env pointer so it will be freed on normal exit */
	register_envp_ref(&my_env);
	set_locale_c(&my_env);
	setup_terminal_pgrp();
	last_status = run_shell_loop(&my_env);
	rl_clear_history();
	free_envp(my_env);
	unregister_envp_ref();
	return (last_status);
}

static void	set_locale_c(char ***my_env_ref)
{
	int idx;

	if (!my_env_ref || !*my_env_ref)
		return ;
	idx = env_find_index(*my_env_ref, "LC_ALL");
	if (idx >= 0)
	{
		free((*my_env_ref)[idx]);
		(*my_env_ref)[idx] = env_make_entry("LC_ALL", "C");
	}
	else
	{
		char *entry = env_make_entry("LC_ALL", "C");
		if (entry)
			env_append_entry(my_env_ref, entry);
	}
}

static void	setup_terminal_pgrp(void)
{
	if (!isatty(STDIN_FILENO))
		return ;
	(void)setpgid(0, 0);
	if (tcgetpgrp(STDIN_FILENO) == -1)
		ioctl(STDIN_FILENO, TIOCSCTTY, 0);
	(void)tcsetpgrp(STDIN_FILENO, getpgrp());
}

static int	run_shell_loop(char ***my_env_ref)
{
	char *line;
	int last_status;

	last_status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (line[0] != '\0')
			last_status = process_line(line, my_env_ref, last_status);
		if (get_exit_requested())
		{
			free(line);
			return (last_status);
		}
		free(line);
	}
	return (0);
}
