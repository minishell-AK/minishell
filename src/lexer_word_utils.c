/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:30:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2025/12/19 20:32:43 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>

int	collect_quoted(t_word_ctx *ctx, size_t *i, char quote)
{
	size_t		start;
	t_seg_type	type;
	t_seg_ctx	seg_ctx;

	start = ++(*i);
	while (*i < ctx->len && ctx->line[*i] != quote)
		(*i)++;
	if (*i >= ctx->len)
		return (-2);
	if (quote == '\'')
		type = SEG_SINGLE_QUOTED;
	else
		type = SEG_DOUBLE_QUOTED;
	seg_ctx.segs = ctx->segs;
	seg_ctx.last = ctx->last;
	seg_ctx.type = type;
	seg_ctx.src = ctx->line + start;
	seg_ctx.len = *i - start;
	if (add_seg_to_list(&seg_ctx) < 0)
		return (-1);
	(*i)++;
	return (0);
}

int	collect_unquoted(t_word_ctx *ctx, size_t *i)
{
	size_t		start;
	t_seg_ctx	seg_ctx;

	start = *i;
	while (*i < ctx->len && !is_space_char((unsigned char)ctx->line[*i])
		&& ctx->line[*i] != '|' && ctx->line[*i] != '<'
		&& ctx->line[*i] != '"' && ctx->line[*i] != '\'')
		(*i)++;
	seg_ctx.segs = ctx->segs;
	seg_ctx.last = ctx->last;
	seg_ctx.type = SEG_UNQUOTED;
	seg_ctx.src = ctx->line + start;
	seg_ctx.len = *i - start;
	if (add_seg_to_list(&seg_ctx) < 0)
		return (-1);
	return (0);
}

int	process_quoted_word(t_word_ctx *ctx, size_t *i)
{
	char	quote;
	int		res;

	quote = ctx->line[*i];
	res = collect_quoted(ctx, i, quote);
	return (res);
}

void	free_seg_list(t_seg *segs)
{
	t_seg	*it;
	t_seg	*n;

	it = segs;
	while (it)
	{
		n = it->next;
		free(it->str);
		free(it);
		it = n;
	}
}
