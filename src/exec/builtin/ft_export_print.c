/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakubo-l <kakubo-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:51:59 by kakubo-l          #+#    #+#             */
/*   Updated: 2026/01/19 10:42:18 by kakubo-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_env_array(char **arr, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	while (++i < count - 1)
	{
		j = i;
		while (++j < count)
		{
			if (ft_strncmp(arr[i], arr[j], ft_strlen(arr[i])) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

static char	**duplicate_and_sort_env(char **envp)
{
	int		count;
	char	**sorted;
	int		i;

	count = count_size_array_char(envp);
	sorted = malloc(sizeof(char *) * (count + 1));
	if (!sorted)
		return (NULL);
	i = -1;
	while (++i < count)
		sorted[i] = envp[i];
	sorted[count] = NULL;
	sort_env_array(sorted, count);
	return (sorted);
}

static	void	print_env_array(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		printf("declare -x %s\n", arr[i]);
}

void	print_sorted_env(char **envp)
{
	char	**sorted;

	sorted = duplicate_and_sort_env(envp);
	if (!sorted)
		return ;
	print_env_array(sorted);
	free(sorted);
}
