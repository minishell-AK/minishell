/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_core_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:56:58 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 10:45:19 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_var(t_exp *ctx, const char *s, size_t *i)
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

int	handle_braced_match(t_exp *ctx, const char *s, size_t *i, size_t j)
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

int	expand_braced_var(t_exp *ctx, const char *s, size_t *i)
{
	size_t	j;

	j = *i + 2;
	while (s[j] && (ft_isalnum((unsigned char)s[j]) || s[j] == '_'))
		j++;
	if (s[j] == '}')
		return (handle_braced_match(ctx, s, i, j));
	return (-1);
}

int	expand_dollar_literal(t_exp *ctx, const char *s, size_t *i)
{
	(void)s;
	if (!expand_char(ctx))
		return (0);
	ctx->out[ctx->out_len++] = '$';
	(*i)++;
	return (1);
}

int	process_expand_char(t_exp *ctx, const char *s, size_t *i)
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
