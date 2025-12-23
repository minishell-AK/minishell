/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:03:07 by kakubo-l          #+#    #+#             */
/*   Updated: 2025/12/19 20:30:43 by kyoshi           ###   ########.fr       */
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

/* redirection handling moved to lexer_helpers.c to reduce function size */

/* redirection handling moved to lexer_helpers.c to reduce function size */

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

t_token	*lexer_tokenize(const char *line)
{
	size_t	i;
	size_t	len;
	t_token	*head;

	i = 0;
	len = 0;
	if (line)
		len = strlen(line);
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
	return (head);
}
