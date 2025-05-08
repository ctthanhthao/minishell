/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:44:16 by thchau            #+#    #+#             */
/*   Updated: 2025/05/08 22:03:08 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	env_builtin(char **envp)
{
	int	i;

	i = 0;
	ft_printf("env builtin is called...\n");
	while (envp[i])
	{
		ft_printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}
