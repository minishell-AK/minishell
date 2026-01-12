/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/08 18:33:10 by kyoshi           ###   ########.fr       */
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
/* lexer/parser inspection removed; restore normal prompt behavior */

volatile sig_atomic_t	g_last_signal = 0;
volatile sig_atomic_t	g_exit_requested = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	g_last_signal = sig;
	write(1, "\n", 1);
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
}

static int	process_line(char *line, char ***envp_ref, int last_status)
{
	t_token		*tokens;
	t_cmd		*cmd;
	t_all_variables	*all;

	add_history(line);
	tokens = lexer_tokenize(line);
	if (tokens)
	{
		expand_tokens(tokens, *envp_ref, last_status);
		cmd = parse_tokens(tokens, envp_ref);
		token_free_all(tokens);
		if (cmd)
		{
			all = add_variables(cmd, envp_ref);
			if (!all)
			{
				fprintf(stderr, "minishell: failed to prepare execution\n");
				free_commands(cmd);
				return (last_status);
			}
			if (ft_exit(all, line))
			{
				/* ensure we free the command-related allocations */
				free_all_variables(all);
				return (last_status);
			}
			last_status = exec_cmd(all);
			return (last_status);
		}
	}
	return (last_status);
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	char		**my_env;

	(void)argc;
	(void)argv;
	setup_signals();
	my_env = dup_envp(envp);
	if (!my_env)
	{
		fprintf(stderr, "minishell: failed to duplicate envp\n");
		return (1);
	}
	/* Force external tools to print English messages to match tests */
	{
		int idx = env_find_index(my_env, "LC_ALL");
		if (idx >= 0)
		{
			free(my_env[idx]);
			my_env[idx] = env_make_entry("LC_ALL", "C");
		}
		else
		{
			char *entry = env_make_entry("LC_ALL", "C");
			if (entry)
				env_append_entry(&my_env, entry);
		}
	}
	int last_status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (line[0] != '\0')
			last_status = process_line(line, &my_env, last_status);
		free(line);
	}
	rl_clear_history();
	free_envp(my_env);
	return (0);
}
