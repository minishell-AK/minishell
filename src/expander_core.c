/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 00:00:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/14 23:56:42 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "../libft/libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int	expand_dollar(t_exp *ctx, const char *s, size_t *i);

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

static int	handle_braced_match(t_exp *ctx, const char *s, size_t *i, size_t j)
{
	size_t	namelen;
	char	*name;
	char	*val;

	namelen = j - (*i + 2);
	name = malloc(namelen + 1);
	if (!name)
		return (0);
	memcpy(name, s + *i + 2, namelen);
	name[namelen] = '\0';
	val = get_env_value(ctx->envp, name);
	if (!val)
		val = "";
	free(name);
	if (!expand_buf(ctx, val))
		return (0);
	*i = j + 1;
	return (1);
}

static int	expand_braced_var(t_exp *ctx, const char *s, size_t *i)
{
	size_t	j;

	j = *i + 2;
	while (s[j] && (ft_isalnum((unsigned char)s[j]) || s[j] == '_'))
		j++;
	if (s[j] == '}')
		return (handle_braced_match(ctx, s, i, j));
	return (-1);
}

static int	expand_dollar_literal(t_exp *ctx, const char *s, size_t *i)
{
	(void)s;
	if (!expand_char(ctx))
		return (0);
	ctx->out[ctx->out_len++] = '$';
	(*i)++;
	return (1);
}

static int	process_expand_char(t_exp *ctx, const char *s, size_t *i)
{
	if (s[*i] == '$')
	{
		if (!expand_dollar((t_exp *)ctx, s, i))
			return (0);
		return (1);
	}
	if (!expand_char(ctx))
		return (0);
	ctx->out[ctx->out_len++] = s[(*i)++];
	return (1);
}

int	expand_dollar(t_exp *ctx, const char *s, size_t *i)
{
	int	r;

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
		if (!process_expand_char(&ctx, s, &i))
			return (free(ctx.out), NULL);
	}
	ctx.out[ctx.out_len] = '\0';
	return (ctx.out);
}
