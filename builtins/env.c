/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:44:16 by thchau            #+#    #+#             */
/*   Updated: 2025/05/21 19:14:16 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	env_builtin(char **envp)
{
	int		i;
	char	*eq;

	i = 0;
	ft_printf("env builtin is called...\n");
	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq)
			ft_printf("%s\n", envp[i]);
		i++;
	}
	return (CMD_SUCCESS);
}
