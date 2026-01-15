/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created: 2026/01/15 17:00:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 17:00:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_helpers.h"
#include "parser.h"
#include "exec.h"
#include "lexer.h"
#include "minishell.h"
#include "../libft/libft.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/ioctl.h>
#include <termios.h>
/* signal helpers moved to src/main_signals.c */

int	process_line(char *line, char ***envp_ref, int last_status)
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

t_all_variables	*prepare_execution(char *line, char ***envp_ref,
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

void	set_locale_c(char ***my_env_ref)
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

void	setup_terminal_pgrp(void)
{
    if (!isatty(STDIN_FILENO))
        return ;
    (void)setpgid(0, 0);
    if (tcgetpgrp(STDIN_FILENO) == -1)
        ioctl(STDIN_FILENO, TIOCSCTTY, 0);
    (void)tcsetpgrp(STDIN_FILENO, getpgrp());
}

int	run_shell_loop(char ***my_env_ref)
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
