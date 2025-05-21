/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:44:52 by thchau            #+#    #+#             */
/*   Updated: 2025/05/21 10:46:05 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand_one_var(char **p, int last_status, char **env)
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