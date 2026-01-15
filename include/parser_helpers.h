/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created: 2026/01/15 12:30:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 12:30:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HELPERS_H
# define PARSER_HELPERS_H

# include "parser.h"

int	has_assignment(const char *s);
int	ft_extract_name_value(const char *raw, char **name, char **value);
int	assign_env_from_raw(const char *raw, t_cmd **head, char ***envp);
t_token	*handle_assignment(t_token *tk, t_cmd **head, char ***envp);

#endif
