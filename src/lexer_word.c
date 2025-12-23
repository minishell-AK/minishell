/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 18:40:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2025/12/19 20:31:16 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

static int	should_break(const char *line, size_t i)
{
	if (is_space_char((unsigned char)line[i]))
		return (1);
	if (line[i] == '|' || line[i] == '<' || line[i] == '>')
		return (1);
	return (0);
}

static int	handle_quoted(t_word_ctx *ctx, size_t *i)
{
	int	res;

	res = process_quoted_word(ctx, i);
	if (res < 0)
		return (-1);
	return (0);
}

int	collect_word(const char *line, size_t *i, size_t len, t_token **head)
{
	t_seg		*segs;
	t_seg		*last;
	t_word_ctx	ctx;

	segs = NULL;
	last = NULL;
	ctx.line = line;
	ctx.len = len;
	ctx.segs = &segs;
	ctx.last = &last;
	while (*i < len)
	{
		if (line[*i] == '\'' || line[*i] == '"')
		{
			if (handle_quoted(&ctx, i) < 0)
				return (-1);
			continue ;
		}
		if (should_break(line, *i))
			break ;
		if (collect_unquoted(&ctx, i) < 0)
			return (-1);
	}
	return (finalize_token(segs, head));
}
