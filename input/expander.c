/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 10:23:05 by thchau            #+#    #+#             */
/*   Updated: 2025/05/20 12:15:46 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*get_env_value(const char *name, char **env)
{
	int		len;
	char	*eq;
	int		i;

	i = 0;
	while (env[i])
	{
		eq = ft_strchr(env[i], '=');
		if (!eq)
		{
			i++;
			continue ;
		}
		len = eq - env[i];
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '='
				&& name[len] == '\0')
			return (eq + 1);
		i++;
	}
	return ("");
}

static char	*expand_one_var(char **p, int last_status, char **env)
{
	const char	*start;
	int			len;
	char		*var_name;
	char		*value;

	start = *p + 1;
	if (*start == '?')
	{
		(*p) += 2;
		return (ft_itoa(last_status));
	}
	len = 0;
	while (start[len] && (ft_isalnum(start[len]) || start[len] == '_'))
		len++;
	var_name = ft_substr(start, 0, len);
	value = ft_strdup(get_env_value(var_name, env));
	free(var_name);
	*p += len + 1;
	return (value);
}

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
	int	start;
	char	*s;

	start = 0;
	s = *p;
	while (s[start] && (s[start] != '$' || in_single))
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

char	*expand_dollar(const char *arg, int last_status, char **env)
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
	ft_printf("The arg is %s\n", arg);
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
	return (result);
}
