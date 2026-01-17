/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 02:00:29 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/17 02:30:27 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HELPERS_H
# define PARSER_HELPERS_H

# include "parser.h"

int		has_assignment(const char *s);
int		ft_extract_name_value(const char *raw, char **name, char **value);
int		assign_env_from_raw(const char *raw, t_cmd **head, char ***envp);
t_token	*handle_assignment(t_token *tk, t_cmd **head, char ***envp);

#endif
