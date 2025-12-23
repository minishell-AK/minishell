/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_seg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:20:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2025/12/18 19:29:13 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdlib.h>
#include <string.h>

static t_seg	*create_node(const char *src, size_t len, t_seg_type type)
{
	char	*s;
	t_seg	*node;

	s = (char *)malloc(len + 1);
	if (!s)
		return (NULL);
	memcpy(s, src, len);
	s[len] = '\0';
	node = (t_seg *)malloc(sizeof(t_seg));
	if (!node)
	{
		free(s);
		return (NULL);
	}
	node->type = type;
	node->str = s;
	node->next = NULL;
	return (node);
}

int	add_seg_to_list(t_seg_ctx *ctx)
{
	t_seg	*node;

	node = create_node(ctx->src, ctx->len, ctx->type);
	if (!node)
		return (-1);
	if (*ctx->last)
		(*ctx->last)->next = node;
	else
		*ctx->segs = node;
	*ctx->last = node;
	return (0);
}

int	add_seg_with_ctx(t_seg_ctx *ctx)
{
	return (add_seg_to_list(ctx));
}
