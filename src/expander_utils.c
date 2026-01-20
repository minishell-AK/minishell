/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 18:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/20 11:08:25 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_buf(t_exp *ctx, const char *str)
{
	size_t	need;

	need = ctx->out_len + ft_strlen(str) + 1;
	if (need > ctx->cap)
	{
		ctx->cap = need * 2;
		ctx->out = xrealloc(ctx->out, ctx->cap);
		if (!ctx->out)
			return (0);
	}
	ft_strlcpy(ctx->out + ctx->out_len, str, strlen(str) + 1);
	ctx->out_len += ft_strlen(str);
	return (1);
}

int	expand_char(t_exp *ctx)
{
	if (ctx->out_len + 2 > ctx->cap)
	{
		ctx->cap = (ctx->out_len + 2) * 2;
		ctx->out = xrealloc(ctx->out, ctx->cap);
		if (!ctx->out)
			return (0);
	}
	return (1);
}

void	rebuild_raw(t_token *tk)
{
	size_t	total;
	t_seg	*it;
	char	*newraw;

	total = 0;
	it = tk->segs;
	while (it)
	{
		total += strlen(it->str);
		it = it->next;
	}
	newraw = malloc(total + 1);
	if (!newraw)
		return ;
	newraw[0] = '\0';
	it = tk->segs;
	while (it)
	{
		ft_strlcat(newraw, it->str, total + 1);
		it = it->next;
	}
	free(tk->raw);
	tk->raw = newraw;
}
