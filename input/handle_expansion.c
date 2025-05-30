/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 10:23:05 by thchau            #+#    #+#             */
/*   Updated: 2025/05/30 16:15:39 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static bool	contains_unquoted_star(char *str)
{
	bool	in_single;
	bool	in_double;

	in_single = false;
	in_double = false;
	while (*str)
	{
		if (*str == '\'' && !in_double)
			in_single = !in_single;
		else if (*str == '\"' && !in_single)
			in_double = !in_double;
		else if (*str == '*' && !in_single && !in_double)
			return (true);
		str++;
	}
	return (false);
}

static int	file_exists(const char *path)
{
	return (access(path, F_OK) == 0);
}

char **handle_expansion_if_any(char *token, int last_status, char **envp)
{
	char	*expanded;
	char	**wildcards;
	char 	**res;
	char	*file_check;

	expanded = expand_variables(token, last_status, envp);
	if (!expanded)
		return (NULL);
	if (contains_unquoted_star(expanded))
	{
		wildcards = expand_wildcard(expanded);
		free(expanded);
		return (wildcards);
	}
	res = malloc(sizeof(char *) * 2);
	file_check = strip_quotes(expanded);
	if (file_exists(file_check))
		res[0] = file_check;
	else
		res[0] = expanded;
	res[1] = NULL;
	return (res);
}
