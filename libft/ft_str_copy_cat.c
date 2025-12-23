/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_copy_cat.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 22:02:18 by kyoshi            #+#    #+#             */
/*   Updated: 2025/12/20 03:40:57 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_str_copy_cat(char *new_str, const char *s1, const char *s2)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (s1)
	{
		while (s1[i])
		{
			new_str[i] = s1[i];
			i++;
		}
	}
	j = 0;
	if (s2)
	{
		while (s2[j])
		{
			new_str[i + j] = s2[j];
			j++;
		}
	}
	new_str[i + j] = '\0';
	return (new_str);
}
