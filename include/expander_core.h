/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_core.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 02:49:07 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/17 02:49:19 by kakubo-l         ###   ########.fr       */
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
