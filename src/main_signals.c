/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_signals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created: 2026/01/15 17:30:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 17:30:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_helpers.h"
#include "minishell.h"
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>

volatile sig_atomic_t	g_last_signal = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	g_last_signal = sig;
	write(1, "^C\n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigquit_handler(int sig)
{
	(void)sig;
}

void	setup_signals(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		perror("sigaction");
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sigquit_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		perror("sigaction");

	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);

	rl_catch_signals = 0;
}
