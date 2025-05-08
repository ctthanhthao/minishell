/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:09:21 by thchau            #+#    #+#             */
/*   Updated: 2025/05/08 21:50:17 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	swap(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

char	**selection_sort(char **ar)
{
	int	min_idx;
	int	i;
	int	j;

	if (!ar || !*ar)
		return (ar);
	i = 0;
	j = 0;
	while (ar[i])
	{
		min_idx = i;
		j = i + 1;
		while (ar[j])
		{
			if (ft_strcmp(ar[i], ar[j]) > 0)
				min_idx = j;
			j++;
		}
		if (i != min_idx)
			swap(&ar[i], &ar[min_idx]);
		i++;
	}
	return (ar);
}
/*#include <stdio.h>
#include "utils.c"
int main()
{
	char *ar[] = {"TERM", "TEAM", "Hello", "world", NULL};
	char **re = selection_sort(ar);
	int i = 0;
	while (re[i])
	{
		printf("%s\n", re[i]);
		i++;
	}
	return (0);
}*/