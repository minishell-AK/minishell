/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_finalize_helpers.h                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copilot <copilot@ai>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:00:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 16:00:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_WORD_FINALIZE_HELPERS_H
# define LEXER_WORD_FINALIZE_HELPERS_H

# include "lexer.h"

void	determine_token_flags(t_token *t, t_seg *segs);
void	collect_token_flags(t_seg *segs, int *all_single, int *seen_double);
void	set_token_flags(t_token *t, t_seg *segs);
size_t	calc_total_len(t_seg *segs);
int	build_raw_string(t_token *t, t_seg *segs, size_t len);
int	finalize_empty_token(t_token **head);

#endif
