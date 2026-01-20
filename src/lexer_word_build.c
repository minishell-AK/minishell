/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_build.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:57:39 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/20 11:23:40 by kyoshi           ###   ########.fr       */
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
	char	*buf;

	t->raw = NULL;
	{
		buf = malloc(len + 1);
		if (!buf)
			return (-1);
		buf[0] = '\0';
		it = segs;
		while (it)
		{
			ft_strlcat(buf, it->str, len + 1);
			it = it->next;
		}
		t->raw = buf;
	}
	return (0);
}
