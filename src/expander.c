/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 21:53:24 by kakubo-l          #+#    #+#             */
/*   Updated: 2025/12/20 03:42:43 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "../libft/libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int	expand_var(t_exp *ctx, const char *s, size_t *i)
{
	size_t	j;
	size_t	namelen;
	char	*name;
	char	*val;

	j = *i + 1;
	while (s[j] && (ft_isalnum((unsigned char)s[j]) || s[j] == '_'))
		j++;
	namelen = j - (*i + 1);
	name = malloc(namelen + 1);
	if (!name)
		return (0);
	memcpy(name, s + *i + 1, namelen);
	name[namelen] = '\0';
	val = get_env_value(ctx->envp, name);
	if (!val)
		val = "";
	free(name);
	if (!expand_buf(ctx, val))
		return (0);
	*i = j;
	return (1);
}

static int	expand_dollar(t_exp *ctx, const char *s, size_t *i)
{
	if (s[*i + 1] == '?')
		return (expand_status(ctx, i));
	if (ft_isalpha((unsigned char)s[*i + 1]) || s[*i + 1] == '_')
		return (expand_var(ctx, s, i));
	if (!expand_char(ctx))
		return (0);
	ctx->out[ctx->out_len++] = '$';
	(*i)++;
	return (1);
}

char	*expand_line(const char *s, char **envp, int last_status)
{
	t_exp	ctx;
	size_t	i;

	ctx.envp = envp;
	ctx.last_status = last_status;
	ctx.cap = strlen(s) + 1;
	ctx.out = malloc(ctx.cap);
	if (!ctx.out)
		return (NULL);
	ctx.out_len = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '$')
		{
			if (!expand_dollar(&ctx, s, &i))
				return (free(ctx.out), NULL);
			continue ;
		}
		if (!expand_char(&ctx))
			return (free(ctx.out), NULL);
		ctx.out[ctx.out_len++] = s[i++];
	}
	ctx.out[ctx.out_len] = '\0';
	return (ctx.out);
}

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
