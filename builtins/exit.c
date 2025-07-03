/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:25:39 by thchau            #+#    #+#             */
/*   Updated: 2025/07/03 19:24:02 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exit_builtin(t_cmd *cmd, int status, char ***envp)
{
	if (!cmd)
		exit(CMD_FAILURE);
	if (!cmd->argv[1])
	{
		free_cmd(cmd);
		free_split(*envp);
		exit(status);
	}
	if (cmd->argv[2])
	{
		free_cmd(cmd);
		free_split(*envp);
		log_errno("too many arguments");
		exit(CMD_FAILURE);
	}
	status = ft_atoi(cmd->argv[1]);
	if (status == 0 && ft_strcmp(cmd->argv[1], "0") != 0)
	{
		log_errno("numeric argument required");
		status = 2;
	}
	free_cmd(cmd);
	free_split(*envp);
	exit(status);
}
