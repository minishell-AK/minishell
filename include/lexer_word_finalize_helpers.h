/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_finalize_helpers.h                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 10:39:56 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 10:40:00 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_WORD_FINALIZE_HELPERS_H
# define LEXER_WORD_FINALIZE_HELPERS_H

# include "lexer.h"

void	determine_token_flags(t_token *t, t_seg *segs);
void	collect_token_flags(t_seg *segs, int *all_single, int *seen_double);
void	set_token_flags(t_token *t, t_seg *segs);
size_t	calc_total_len(t_seg *segs);
int		build_raw_string(t_token *t, t_seg *segs, size_t len);
int		finalize_empty_token(t_token **head);

#endif
