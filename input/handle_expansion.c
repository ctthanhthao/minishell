/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 10:23:05 by thchau            #+#    #+#             */
/*   Updated: 2025/05/27 22:09:04 by thchau           ###   ########.fr       */
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

char **handle_expansion_if_any(char *token, int last_status, char **envp, bool first)
{
	char	*expanded;
	char	**wildcards;
	char 	**res;

	expanded = expand_variables(token, last_status, envp, first);
	if (!expanded)
		return (NULL);
	if (contains_unquoted_star(expanded))
	{
		wildcards = expand_wildcard(expanded);
		free(expanded);
		return (wildcards);
	}
	res = malloc(sizeof(char *) * 2);
	res[0] = expanded;
	res[1] = NULL;
	return (res);
}
