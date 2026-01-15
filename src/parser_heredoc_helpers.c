/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:59:44 by kyoshi            #+#    #+#             */
/*   Updated: 2026/01/15 19:59:45 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "parser.h"
#include "parser_heredoc_helpers.h"
#include "minishell.h"
#include "../libft/libft.h"
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

/* parser heredoc helpers were split into:
   - src/parser_heredoc_spawn.c
   - src/parser_heredoc_wait.c
   - src/parser_heredoc_io.c
   The implementations moved to those files. */

#include "parser_heredoc_helpers.h"
