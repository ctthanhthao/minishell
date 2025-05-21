/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 10:23:05 by thchau            #+#    #+#             */
/*   Updated: 2025/05/21 12:52:36 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*static bool	handle_backslash(char **p, char **tmp, int in_single)
{
	char	*s;
	
	s = *p;
	if (*s == '\\')
	{
		if (in_single && (s + 1) && *(s + 1) == '$')
		{
			*tmp = ft_substr(*p, 0, 2);
			(*p) += 2;
		} 
		else if (in_single)
		{
			*tmp = ft_substr(*p, 0, 1);
			(*p) ++;
		}
		else
		{
			*tmp = ft_substr((*p) + 1, 0, 1);
			(*p) += 2;
		}
		return (true);
	}
	return (false);
}*/

static bool	handle_quotes(char **p, int *in_double, int *in_single, char **tmp)
{
	char	c;

	c = **p;
	if (c == '\'' && !*in_double)
	{
		*in_single = !*in_single;
		*tmp = ft_substr(*p, 0, 1);
		(*p)++;
		return (true);
	}
	else if (c == '"' && !*in_single)
	{
		*in_double = !*in_double;
		*tmp = ft_substr(*p, 0, 1);
		(*p)++;
		return (true);
	}
	return (false);
}

static void	handle_text(char **p, int in_double, int in_single, char **tmp)
{
	int		start;
	char	*s;

	// ft_printf("handle_backslash.....\n");
	// if (handle_backslash(p, tmp, in_single))
	// 	return ;
	start = 0;
	s = *p;
	ft_printf("handle_text......\n");
	while (s[start] && ((s[start] != '$') || in_single))
	{
		if (s[start] == '\'' && !in_double)
			break ;
		if (s[start] == '"' && !in_single)
			break ;
		start++;
	}
	*tmp = ft_substr(s, 0, start);
	(*p) += start;
}
/**
 * It's supposed to apply to an argv in cmd,
 * - If argv contains valid env variables, new argv is returned after 
 * those variables are replaced by their value.
 * - Otherwise, original argv is returned. 
 * It supports
 * + Quotes (' should disable expansion)
 * + Recursive expansion ($FOO_$BAR)
 * + Skipping invalid variable names ($5X or $!)
 */

char	*handle_expansion_if_any(const char *arg, int last_status, char **env)
{
	char	*p ;
	char	*result;
	char	*tmp;
	int		in_single;
	int		in_double;

	p = (char *)arg;
	result = ft_calloc(1, 1);
	in_single = 0;
	in_double = 0;
	//ft_printf("expand_dollar arg is %s\n", arg);
	while (*p)
	{
		if (handle_quotes(&p, &in_double, &in_single, &tmp))
			;
		else if (*p == '$' && !in_single)
			tmp = expand_one_var(&p, last_status, env);
		else
			handle_text(&p, in_double, in_single, &tmp);
		result = ft_strjoin_free(result, tmp);
		free(tmp);
	}
	//ft_printf("expand_dollar result is %s\n", result);
	tmp = ft_strtrim(result, "\"\'");
	free(result);
	return (tmp);
}
