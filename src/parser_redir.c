/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 20:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2025/12/19 21:25:53 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "../libft/libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>

static t_redir_type	get_redir_type(t_token_type type)
{
	t_redir_type	rt;

	if (type == TOK_REDIR_IN)
		rt = REDIR_IN;
	else if (type == TOK_REDIR_OUT)
		rt = REDIR_OUT;
	else if (type == TOK_REDIR_APPEND)
		rt = APPEND;
	else
		rt = HEREDOC;
	return (rt);
}

static char	*get_redir_target(t_redir_type rt, t_token *next, char **envp)
{
	char	*target;

	if (rt == HEREDOC)
		target = create_heredoc(next, envp);
	else
		target = ft_strdup(next->raw);
	return (target);
}

static int	process_redir(t_cmd **cur, t_redir_type rt, char *target)
{
	int	result;

	result = add_redir(*cur, rt, target);
	free(target);
	return (result);
}

t_token	*parse_redir_token(t_token *tk, t_cmd **head, t_cmd **cur,
	char **envp)
{
	t_redir_type	rt;
	t_token			*next;
	char			*target;

	rt = get_redir_type(tk->type);
	next = tk->next;
	if (!next || next->type != TOK_WORD)
	{
		ft_putendl_fd("parse error: redirection without target", 2);
		free_commands(*head);
		return (NULL);
	}
	if (!ensure_cmd(head, cur))
		return (NULL);
	target = get_redir_target(rt, next, envp);
	if (!target || process_redir(cur, rt, target) == -1)
	{
		free_commands(*head);
		return (NULL);
	}
	return (next->next);
}
