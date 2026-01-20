/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 17:58:01 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/20 11:08:26 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_word_token(const char *line, size_t start,
					size_t end, t_token **head)
{
	char	*raw;
	t_token	*t;

	raw = malloc(end - start + 1);
	if (!raw)
		return (-1);
	memcpy(raw, line + start, end - start);
	raw[end - start] = '\0';
	t = token_new(TOK_WORD, NULL);
	if (!t)
	{
		free(raw);
		return (-1);
	}
	t->raw = raw;
	token_append(head, t);
	return (0);
}

static int	make_and_append(t_token **head, t_token_type type,
					const char *s, size_t *i)
{
	t_token	*t;

	t = token_new(type, NULL);
	if (!t)
		return (-1);
	t->raw = ft_strdup(s);
	if (!t->raw)
	{
		free(t);
		return (-1);
	}
	token_append(head, t);
	*i += strlen(s);
	return (1);
}

int	handle_redir(const char *line, size_t *i, size_t len, t_token **head)
{
	if (*i >= len)
		return (0);
	if (line[*i] == '>')
	{
		if (*i + 1 < len && line[*i + 1] == '>')
			return (make_and_append(head, TOK_REDIR_APPEND, ">>", i));
		return (make_and_append(head, TOK_REDIR_OUT, ">", i));
	}
	if (line[*i] == '<')
	{
		if (*i + 1 < len && line[*i + 1] == '<')
			return (make_and_append(head, TOK_HEREDOC, "<<", i));
		return (make_and_append(head, TOK_REDIR_IN, "<", i));
	}
	return (0);
}
