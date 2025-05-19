/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:08:16 by thchau            #+#    #+#             */
/*   Updated: 2025/05/19 13:23:22 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	echo_builtin(t_cmd *cmd, int *status)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	ft_printf("echo builtin is called...\n");
	if (cmd->argv[i] && !ft_strcmp(cmd->argv[i], "-n"))
	{
		newline = 0;
		i++;
	}
	while (cmd->argv[i])
	{
		ft_printf("%s", cmd->argv[i]);
		if (cmd->argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	*status = 0;
	return (CMD_SUCCESS);
}
