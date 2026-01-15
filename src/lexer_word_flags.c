/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_flags.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:58:01 by kyoshi            #+#    #+#             */
/*   Updated: 2026/01/15 19:58:02 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "lexer.h"
#include "lexer_word_finalize_helpers.h"

void	determine_token_flags(t_token *t, t_seg *segs)
{
	int all_single;
	int seen_double;

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

void	collect_token_flags(t_seg *segs, int *all_single, int *seen_double)
{
	t_seg *it;

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
