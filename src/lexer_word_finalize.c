/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_finalize.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:30:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/14 23:56:42 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "../libft/libft.h"
#include <stdlib.h>
#include <string.h>

static void	determine_token_flags(t_token *t, t_seg *segs);
static void	collect_token_flags(t_seg *segs, int *all_single, int *seen_double);

static void	set_token_flags(t_token *t, t_seg *segs)
{
	determine_token_flags(t, segs);
}

static void	determine_token_flags(t_token *t, t_seg *segs)
{
	int	all_single;
	int	seen_double;

	collect_token_flags(segs, &all_single, &seen_double);
	if (all_single)
		t->no_expand = 1;
	else
		t->no_expand = 0;
	if (seen_double)
		t->in_double = 1;
	else
		t->in_double = 0;
}

static void	collect_token_flags(t_seg *segs, int *all_single, int *seen_double)
{
	t_seg	*it;

	*all_single = 1;
	*seen_double = 0;
	it = segs;
	while (it)
	{
		if (it->type != SEG_SINGLE_QUOTED)
			*all_single = 0;
		if (it->type == SEG_DOUBLE_QUOTED)
			*seen_double = 1;
		it = it->next;
	}
}

static size_t	calc_total_len(t_seg *segs)
{
	t_seg	*it;
	size_t	len;

	len = 0;
	it = segs;
	while (it)
	{
		len += strlen(it->str);
		it = it->next;
	}
	return (len);
}

static int	build_raw_string(t_token *t, t_seg *segs, size_t len)
{
	t_seg	*it;

	t->raw = (char *)malloc(len + 1);
	if (!t->raw)
	{
		free_seg_list(segs);
		free(t);
		return (-1);
	}
	t->raw[0] = '\0';
	it = segs;
	while (it)
	{
		ft_strlcat(t->raw, it->str, len + 1);
		it = it->next;
	}
	return (0);
}

static int	finalize_empty_token(t_token **head)
{
	t_token	*t;

	t = token_new(TOK_WORD, "");
	if (!t)
		return (-1);
	token_append(head, t);
	return (0);
}

int	finalize_token(t_seg *segs, t_token **head)
{
	t_token	*t;
	size_t	len;

	if (!segs)
		return (finalize_empty_token(head));
	t = token_new(TOK_WORD, NULL);
	if (!t)
	{
		free_seg_list(segs);
		return (-1);
	}
	t->segs = segs;
	set_token_flags(t, segs);
	len = calc_total_len(segs);
	if (build_raw_string(t, segs, len) < 0)
		return (-1);
	token_append(head, t);
	return (0);
}
