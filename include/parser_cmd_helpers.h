/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_helpers.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created: 2026/01/15 14:20:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/15 14:20:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_CMD_HELPERS_H
# define PARSER_CMD_HELPERS_H

# include "parser.h"

char	**copy_old_args(char **newargv, char **oldargs, size_t cnt);
int	reject_multiple_out_redirs(t_cmd *cmd);

#endif
