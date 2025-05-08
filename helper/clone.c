/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clone.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:10:16 by thchau            #+#    #+#             */
/*   Updated: 2025/05/08 22:17:19 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	free_created_elements(char **ar, int to_pos)
{
	int	k;

	k = 0;
	while (k < to_pos)
	{
		free(ar[k]);
		ar[k] = NULL;
		k++;
	}
}

char	**clone_arr(char **ar)
{
	int		len;
	int		i;
	char	**copy;

	if (!ar || !*ar)
		return (NULL);
	len = 0;
	i = 0;
	while (ar[len])
		len++;
	copy = malloc(sizeof(char *) * (len + 1));
	if (!copy)
		return (NULL);
	while (i < len)
	{
		copy[i] = ft_strdup(ar[i]);
		if (!copy[i])
		{
			free_created_elements(copy, i);
			free(copy);
			return (NULL);
		}
		i++;
	}
	return (copy[i] = NULL, copy);
}
