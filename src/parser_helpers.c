/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 20:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2025/12/20 04:06:48 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "../libft/libft.h"
#include "minishell.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int	has_assignment(const char *s)
{
	size_t	i;
	char	*eq;

	if (!s || !s[0])
		return (0);
	eq = ft_strchr(s, '=');
	if (!eq)
		return (0);
	if (eq == s)
		return (0);
	if (!ft_isalpha((int)s[0]) && s[0] != '_')
		return (0);
	i = 1;
	while (i < (size_t)(eq - s))
	{
		if (!ft_isalnum((int)s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static t_token	*handle_assignment(t_token *tk, t_cmd **head,
				char ***envp)
{
	char	*eq;
	char	*name;
	char	*value;

	eq = ft_strchr(tk->raw, '=');
	if (!eq)
		return (tk->next);
	name = ft_substr(tk->raw, 0, (size_t)(eq - tk->raw));
	value = ft_strdup(eq + 1);
	if (!name || !value)
	{
		free_commands(*head);
		return (NULL);
	}
	if (!set_env_var(envp, name, value))
	{
		free(name);
		free(value);
		free_commands(*head);
		return (NULL);
	}
	free(name);
	free(value);
	return (tk->next);
}

t_token	*parse_word_token(t_token *tk, t_cmd **head, t_cmd **cur,
		char ***envp)
{
	if (!*cur && has_assignment(tk->raw)
		&& (!tk->next || tk->next->type == TOK_END))
		return (handle_assignment(tk, head, envp));
	if (!ensure_cmd(head, cur))
		return (NULL);
	if (add_arg(*cur, tk->raw) == -1)
	{
		free_commands(*head);
		return (NULL);
	}
	return (tk->next);
}

t_token	*parse_pipe_token(t_token *tk, t_cmd **head, t_cmd **cur)
{
	if (!*cur)
	{
		ft_putendl_fd("parse error: pipe with no command before", 2);
		free_commands(*head);
		return (NULL);
	}
	(*cur)->next = cmd_new();
	if (!(*cur)->next)
	{
		free_commands(*head);
		return (NULL);
	}
	*cur = (*cur)->next;
	return (tk->next);
}

t_token	*parse_error_token(t_token *tk, t_cmd **head)
{
	const char	*msg;

	if (tk->raw)
		msg = tk->raw;
	else
		msg = "(null)";
	ft_putstr_fd("lexer error token: ", 2);
	ft_putendl_fd(msg, 2);
	free_commands(*head);
	return (NULL);
}
