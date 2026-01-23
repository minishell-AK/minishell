/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_helpers.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 02:30:43 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/23 19:07:41 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_CMD_HELPERS_H
# define PARSER_CMD_HELPERS_H

# include "parser.h"

char	**copy_old_args(char **newargv, char **oldargs, size_t cnt);
int		reject_multiple_out_redirs(t_cmd *cmd);
char	**new_argv_with_arg(char **old, const char *arg);

#endif
