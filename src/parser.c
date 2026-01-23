/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:26:47 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/23 18:42:37 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*dispatch_token(t_token *tk, t_cmd **head, t_cmd **cur,
	char ***envp)
{
	if (tk->type == TOK_WORD)
		return (parse_word_token(tk, head, cur, envp));
	else if (tk->type == TOK_REDIR_IN || tk->type == TOK_REDIR_OUT
		|| tk->type == TOK_REDIR_APPEND || tk->type == TOK_HEREDOC)
		return (parse_redir_token(tk, head, cur, *envp));
	else if (tk->type == TOK_PIPE)
		return (parse_pipe_token(tk, head, cur));
	else if (tk->type == TOK_ERROR)
		return (parse_error_token(tk, head));
	else
		return (tk->next);
}

t_cmd	*parse_tokens(t_token *tokens, char ***envp)
{
	t_cmd	*head;
	t_cmd	*cur;
	t_token	*tk;

	head = NULL;
	cur = NULL;
	tk = tokens;
	while (tk && tk->type != TOK_END)
	{
		tk = dispatch_token(tk, &head, &cur, envp);
		if (!tk)
		{
			return (NULL);
		}
	}
	return (head);
}
