/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:03:07 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/20 21:15:38 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_pipe(const char *line, size_t *i, t_token **head)
{
	t_token	*t;

	if (line[*i] == '|')
	{
		t = token_new(TOK_PIPE, NULL);
		if (!t)
			return (-1);
		t->raw = ft_strdup("|");
		if (!t->raw)
		{
			free(t);
			return (-1);
		}
		token_append(head, t);
		(*i)++;
		return (1);
	}
	return (0);
}

static int	handle_metachar(const char *line, size_t *i, size_t len,
	t_token **head)
{
	int	ret;

	ret = handle_pipe(line, i, head);
	if (ret == -1)
		return (-1);
	if (ret == 1)
		return (1);
	ret = handle_redir(line, i, len, head);
	if (ret == -1)
		return (-1);
	if (ret == 1)
		return (1);
	return (0);
}

/* helpers moved to lexer_helpers.c to respect Norminette function limits */

static int	tokenize_fill(const char *line, size_t len, t_token **head);

static t_token	*lexer_tokenize_core(const char *line, size_t len)
{
	t_token	*head;
	t_token	*end_tok;

	head = NULL;
	if (tokenize_fill(line, len, &head) == -1)
	{
		token_free_all(head);
		return (NULL);
	}
	end_tok = token_new(TOK_END, NULL);
	if (end_tok)
		token_append(&head, end_tok);
	return (head);
}

static int	tokenize_fill(const char *line, size_t len, t_token **head)
{
	size_t	i;
	int		hm;

	i = 0;
	while (i < len)
	{
		if (is_space_char((unsigned char)line[i]))
		{
			i++;
			continue ;
		}
		{
			hm = handle_metachar(line, &i, len, head);
			if (hm == -1)
				return (-1);
			if (hm == 1)
				continue ;
		}
		if (collect_word(line, &i, len, head) != 0)
			return (-1);
	}
	return (0);
}

t_token	*lexer_tokenize(const char *line)
{
	size_t	len;

	len = 0;
	if (line)
		len = strlen(line);
	return (lexer_tokenize_core(line, len));
}
