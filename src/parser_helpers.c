/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 20:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/15 16:16:50 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "parser_helpers.h"
#include "../libft/libft.h"
#include "minishell.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* helper implementations moved to parser_helpers_utils.c */

t_token	*parse_word_token(t_token *tk, t_cmd **head, t_cmd **cur,
		char ***envp)
{
	if (!*cur && has_assignment(tk->raw)
		&& (!tk->next || tk->next->type == TOK_END))
		return (handle_assignment(tk, head, envp));
	if (!ensure_cmd(head, cur))
		return (NULL);
	if (add_arg(*cur, tk->raw) == -1)
	{
		free_commands(*head);
		return (NULL);
	}
	return (tk->next);
}

t_token	*parse_pipe_token(t_token *tk, t_cmd **head, t_cmd **cur)
{
	if (!*cur)
	{
		ft_putendl_fd("parse error: pipe with no command before", 2);
		free_commands(*head);
		return (NULL);
	}
	(*cur)->next = cmd_new();
	if (!(*cur)->next)
	{
		free_commands(*head);
		return (NULL);
	}
	*cur = (*cur)->next;
	return (tk->next);
}

t_token	*parse_error_token(t_token *tk, t_cmd **head)
{
	const char	*msg;

	if (tk->raw)
		msg = tk->raw;
	else
		msg = "(null)";
	ft_putstr_fd("lexer error token: ", 2);
	ft_putendl_fd(msg, 2);
	free_commands(*head);
	return (NULL);
}
