/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_finalize_helpers.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copilot <copilot@ai>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:00:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 16:00:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "lexer_word_finalize_helpers.h"
#include "../libft/libft.h"
#include <stdlib.h>
#include <string.h>

void	set_token_flags(t_token *t, t_seg *segs)
{
    determine_token_flags(t, segs);
}

int	finalize_empty_token(t_token **head)
{
    t_token *t;

    t = token_new(TOK_WORD, "");
    if (!t)
        return (-1);
    token_append(head, t);
    return (0);
}
