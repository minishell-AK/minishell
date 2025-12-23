/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:04:12 by kakubo-l          #+#    #+#             */
/*   Updated: 2025/12/19 20:54:07 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "../libft/libft.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

t_token	*token_new(t_token_type type, const char *str)
{
	t_token	*t;

	t = malloc(sizeof(t_token));
	if (!t)
		return (NULL);
	t->type = type;
	t->raw = NULL;
	t->no_expand = 0;
	t->in_double = 0;
	t->segs = NULL;
	if (str)
	{
		t->raw = ft_strdup(str);
		if (!t->raw)
		{
			free(t);
			return (NULL);
		}
	}
	t->next = NULL;
	return (t);
}

void	token_append(t_token **head, t_token *node)
{
	t_token	*it;

	if (!head || !node)
		return ;
	if (!*head)
	{
		*head = node;
		return ;
	}
	it = *head;
	while (it->next)
		it = it->next;
	it->next = node;
}

static void	free_segments(t_seg *segs)
{
	t_seg	*s;
	t_seg	*sn;

	s = segs;
	while (s)
	{
		sn = s->next;
		if (s->str)
			free(s->str);
		free(s);
		s = sn;
	}
}

void	token_free_all(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		if (head->segs)
			free_segments(head->segs);
		if (head->raw)
			free(head->raw);
		free(head);
		head = tmp;
	}
}

/* debug printing removed */
