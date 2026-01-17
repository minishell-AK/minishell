/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:03:07 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/17 01:09:59 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>

static int	handle_pipe(const char *line, size_t *i, t_token **head)
{
	t_token	*t;

	if (line[*i] == '|')
	{
		t = token_new(TOK_PIPE, "|");
		if (!t)
			return (0);
		token_append(head, t);
		(*i)++;
		return (1);
	}
	return (0);
}

static int	handle_metachar(const char *line, size_t *i, size_t len,
	t_token **head)
{
	if (handle_pipe(line, i, head))
		return (1);
	if (handle_redir(line, i, len, head))
		return (1);
	return (0);
}

/* helpers moved to lexer_helpers.c to respect Norminette function limits */

static t_token	*lexer_tokenize_core(const char *line, size_t len)
{
	size_t	i;
	t_token	*head;
	t_token	*end_tok;

	i = 0;
	head = NULL;
	while (i < len)
	{
		if (is_space_char((unsigned char)line[i]))
		{
			i++;
			continue ;
		}
		if (handle_metachar(line, &i, len, &head))
			continue ;
		if (collect_word(line, &i, len, &head) != 0)
			return (head);
	}
	end_tok = token_new(TOK_END, "");
	if (end_tok)
		token_append(&head, end_tok);
	return (head);
}

t_token	*lexer_tokenize(const char *line)
{
	size_t	len;

	len = 0;
	if (line)
		len = strlen(line);
	return (lexer_tokenize_core(line, len));
}
