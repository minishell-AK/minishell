/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:01:10 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 10:49:18 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* helpers moved to parser_heredoc_helpers.c */

char	*create_heredoc(t_token *delim_token, char **envp)
{
	int		expand;
	char	*tmpfile;
	t_seg	*s;

	expand = 1;
	if (delim_token->segs)
	{
		s = delim_token->segs;
		while (s)
		{
			if (s->type == SEG_SINGLE_QUOTED || s->type == SEG_DOUBLE_QUOTED)
			{
				expand = 0;
				break ;
			}
			s = s->next;
		}
	}
	tmpfile = read_heredoc_lines(delim_token->raw, expand, envp);
	return (tmpfile);
}
