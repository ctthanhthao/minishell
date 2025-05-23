/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:38:06 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/23 09:16:58 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	log_error(const char *error, const char *function)
{
	if (error)
		ft_printf(R "%s\n" RST, error);
	if (function)
		perror(function);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 && !s2)
		return (0);
	if (!s1)
		return (-1);
	if (!s2)
		return (1);
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	is_valid_identifier(const char *s)
{
	if (!s || (!ft_isalpha(*s) && *s != '_'))
		return (0);
	while (*s && *s != '=')
	{
		if (!ft_isalnum(*s) && *s != '_')
			return (0);
		s++;
	}
	return (1);
}

void	print_sorted_env(char **env)
{
	int		i;
	char	*eq;
	char	*key;
	char	**copy;

	if (!env || !*env)
		return ;
	copy = clone_arr(env);
	if (copy == NULL)
		return ;
	copy = selection_sort(copy);
	i = -1;
	while (copy[++i])
	{
		eq = ft_strchr(copy[i], '=');
		if (eq != NULL)
		{
			key = ft_substr(copy[i], 0, eq - copy[i]);
			ft_printf("declare -x %s=\"%s\"\n", key, (eq + 1));
			free(key);
		}
		else
			ft_printf("declare -x %s\n", copy[i]);	
	}
	free_split(copy);
}

void print_env(char **envp)
{
	int i = 0;

	ft_printf("Printing envp:\n");
	while (envp && envp[i])
	{
		ft_printf("[%d]: %s\n", i, envp[i]);
		i++;
	}
}