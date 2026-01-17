/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_build.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:57:39 by kyoshi            #+#    #+#             */
/*   Updated: 2026/01/17 01:08:07 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	calc_total_len(t_seg *segs)
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

int	build_raw_string(t_token *t, t_seg *segs, size_t len)
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
