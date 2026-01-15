/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_core.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created: 2026/01/15 13:40:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 13:40:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_CORE_H
# define EXPANDER_CORE_H

# include <stddef.h>
# include "minishell.h"
# include "lexer.h"

int	expand_dollar(t_exp *ctx, const char *s, size_t *i);

int	expand_var(t_exp *ctx, const char *s, size_t *i);
int	handle_braced_match(t_exp *ctx, const char *s, size_t *i, size_t j);
int	expand_braced_var(t_exp *ctx, const char *s, size_t *i);
int	expand_dollar_literal(t_exp *ctx, const char *s, size_t *i);
int	process_expand_char(t_exp *ctx, const char *s, size_t *i);

#endif
