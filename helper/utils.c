/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:38:06 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/29 10:01:49 by thchau           ###   ########.fr       */
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

void	log_errno(const char *msg)
{
	const char	*err;
	
	if (msg)
	{
		write(2, "minishell", 8);
		write(2, ": ", 2);
		write(2, msg, ft_strlen(msg));
	}
	else
	{
		err = strerror(errno);
		write(2, err, ft_strlen(err));
	}
	write(2, "\n", 1);
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

int	safe_dup2(int oldfd, int newfd, char *error)
{
	if (oldfd == -1)
	{
		log_errno(error);
		return (CMD_FAILURE);
	}
	if (dup2(oldfd, newfd) == -1)
	{
		close(oldfd);
		log_errno("Error duplicating file descriptor");
		return (CMD_FAILURE);
	}
	close(oldfd);
	return (CMD_SUCCESS);
}
