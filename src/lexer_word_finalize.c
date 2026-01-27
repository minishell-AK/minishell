/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_finalize.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:30:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/26 03:48:45 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	{
		free_seg_list(segs);
		free(t);
		return (-1);
	}
	token_append(head, t);
	return (0);
}
