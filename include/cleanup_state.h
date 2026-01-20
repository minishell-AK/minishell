/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_state.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 00:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/20 10:12:48 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEANUP_STATE_H
# define CLEANUP_STATE_H

typedef struct s_cleanup_state
{
	char	***envp_ref;
	int		atexit_registered;
}	t_cleanup_state;

#endif
