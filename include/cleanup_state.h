/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_state.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 00:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/27 22:09:57 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEANUP_STATE_H
# define CLEANUP_STATE_H

typedef struct s_cleanup_state
{
	char	***envp_ref;
	int		atexit_registered;
	int     heredoc_fd;
	char    *heredoc_template;
}	t_cleanup_state;

#endif
