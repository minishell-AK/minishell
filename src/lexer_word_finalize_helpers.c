/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_finalize_helpers.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:57:46 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/21 11:09:51 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_token_flags(t_token *t, t_seg *segs)
{
	determine_token_flags(t, segs);
}

int	finalize_empty_token(t_token **head)
{
	t_token	*t;

	t = token_new(TOK_WORD, NULL);
	if (!t)
		return (-1);
	token_append(head, t);
	return (0);
}
