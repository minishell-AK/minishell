/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 00:00:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 16:27:23 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "../libft/libft.h"
#include "minishell.h"
#include "expander_core.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int	expand_dollar(t_exp *ctx, const char *s, size_t *i)
{
	int r;

	if (s[*i + 1] == '?')
		return (expand_status(ctx, i));
	if (s[*i + 1] == '{')
	{
		r = expand_braced_var(ctx, s, i);
		if (r != -1)
			return (r);
	}
	if (ft_isalpha((unsigned char)s[*i + 1]) || s[*i + 1] == '_')
		return (expand_var(ctx, s, i));
	return (expand_dollar_literal(ctx, s, i));
}

char	*expand_line(const char *s, char **envp, int last_status)
{
	t_exp ctx;
	size_t i;

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
		if (!process_expand_char(&ctx, s, &i))
			return (free(ctx.out), NULL);
	}
	ctx.out[ctx.out_len] = '\0';
	return (ctx.out);
}
