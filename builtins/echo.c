/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:08:16 by thchau            #+#    #+#             */
/*   Updated: 2025/05/08 09:59:01 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	handle_argv(char *arg)
{
	char	*env_value;

	if (arg[0] == '$')
	{
		env_value = getenv(arg + 1);
		if (env_value)
			ft_printf("%s", env_value);
	}
	else
		ft_printf("%s", arg);
}

int echo_builtin(t_cmd *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	ft_printf("echo builtin is called...\n");
	if (cmd->argv[i] && !ft_strncmp(cmd->argv[i], "-n", 2))
	{
		newline = 0;
		i++;
	}
	while (cmd->argv[i])
	{
		handle_argv(cmd->argv[i]);
		if (cmd->argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (0);
}
