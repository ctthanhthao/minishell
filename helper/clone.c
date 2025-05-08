/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clone.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:10:16 by thchau            #+#    #+#             */
/*   Updated: 2025/05/08 21:12:05 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**clone_arr(char **ar)
{
	int		len;
	int		i;
	char	**copy;

	if (!ar || !*ar)
		return (NULL);
	len = 0;
	i = 0;
	while(ar[len])
		len++;
	copy = malloc(sizeof(char *) * (len + 1));
	if (!copy)
		return (NULL);
	while (i < len)
	{
		copy[i] = ft_strdup(ar[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}