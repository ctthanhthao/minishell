/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:42:30 by thchau            #+#    #+#             */
/*   Updated: 2025/05/25 13:05:17 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(const char *cmd)
{
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "pwd"));
}

int	execute_builtin(t_cmd *cmd, char ***envp, int *status)
{
	if (!ft_strcmp(cmd->argv[0], "cd"))
		*status = cd_builtin(cmd);
	else if (!ft_strcmp(cmd->argv[0], "exit"))
	{
		*status = exit_builtin(cmd, envp);
		return (CMD_EXIT);
	}
	else if (!ft_strcmp(cmd->argv[0], "export"))
		*status = export_builtin(cmd, envp);
	else if (!ft_strcmp(cmd->argv[0], "unset"))
		*status = unset_builtin(cmd, envp);
	else if (!ft_strcmp(cmd->argv[0], "echo"))
		*status = echo_builtin(cmd, status);
	else if (!ft_strcmp(cmd->argv[0], "env"))
		*status = env_builtin(*envp);
	else if (!ft_strcmp(cmd->argv[0], "pwd"))
		*status = pwd_builtin();
	else
		*status = 1;
	return (*status);
}
