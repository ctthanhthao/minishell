/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:42:30 by thchau            #+#    #+#             */
/*   Updated: 2025/05/19 10:14:34 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(const char *cmd)
{
	return (!ft_memcmp(cmd, "cd", 2) ||
			!ft_memcmp(cmd, "exit", 4) ||
			!ft_memcmp(cmd, "export", 6) ||
			!ft_memcmp(cmd, "unset", 5) ||
			!ft_memcmp(cmd, "echo", 4) ||
			!ft_memcmp(cmd, "env", 3) ||
			!ft_memcmp(cmd, "pwd", 3));
}

int execute_builtin(t_cmd *cmd, char ***envp, int *status)
{
	if (!ft_memcmp(cmd->argv[0], "cd", 2))
		*status = cd_builtin(cmd);
	else if (!ft_memcmp(cmd->argv[0], "exit", 4))
	{
		*status = exit_builtin(cmd, envp);
		return (CMD_EXIT);
	}
	else if (!ft_memcmp(cmd->argv[0], "export", 6))
		*status = export_builtin(cmd, envp);
	else if (!ft_memcmp(cmd->argv[0], "unset", 5))
		*status = unset_builtin(cmd, envp);
	else if (!ft_memcmp(cmd->argv[0], "echo", 4))
		*status = echo_builtin(cmd, status);
	else if (!ft_memcmp(cmd->argv[0], "env", 3))
		*status = env_builtin(*envp);
	else if (!ft_memcmp(cmd->argv[0], "pwd", 3))
		*status = pwd_builtin();
	else
		*status = 1;
	return (CMD_SUCCESS);
}
