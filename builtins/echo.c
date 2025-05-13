/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:08:16 by thchau            #+#    #+#             */
/*   Updated: 2025/05/09 14:56:15 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	handle_argv(char *arg, int status)
{
	char	*env_value;

	if (arg[0] == '$' && arg[1] == '?')
	{
		ft_printf("%d", status);
		ft_printf("%s", (ft_strchr(arg, '?') + 1));
	}
	else if (arg[0] == '$')
	{
		env_value = getenv(arg + 1);
		if (env_value)
			ft_printf("%s", env_value);
	}
	else
		ft_printf("%s", arg);
}

int	echo_builtin(t_cmd *cmd, int status)
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
		handle_argv(cmd->argv[i], status);
		if (cmd->argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (0);
}
