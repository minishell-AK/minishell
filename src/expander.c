/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 21:53:24 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/17 01:17:12 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "../libft/libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void	expand_segment(t_seg *seg, char **envp, int last_status)
{
	const char	*s;
	char		*result;

	if (seg->type == SEG_SINGLE_QUOTED)
		return ;
	s = seg->str;
	if (!s)
		s = "";
	result = expand_line(s, envp, last_status);
	if (!result)
		return ;
	free(seg->str);
	seg->str = ft_strdup(result);
	free(result);
}

void	expand_tokens(t_token *head, char **envp, int last_status)
{
	t_token	*tk;
	t_seg	*seg;

	tk = head;
	while (tk)
	{
		if (tk->type == TOK_WORD && !tk->no_expand && tk->segs)
		{
			seg = tk->segs;
			while (seg)
			{
				expand_segment(seg, envp, last_status);
				seg = seg->next;
			}
			rebuild_raw(tk);
		}
		tk = tk->next;
	}
}
