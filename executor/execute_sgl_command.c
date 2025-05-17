/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_sgl_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:47:05 by thchau            #+#    #+#             */
/*   Updated: 2025/05/17 19:15:59 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


static int	execute_non_builtin_cmd(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	int		status;
	
	pid = fork();
	if (pid == -1)
		return (log_error("Error happened in fork during execute_single_command",
			"fork"), CMD_FAILURE);
	if (pid == 0)
	{
		if (execve(cmd->argv[0], cmd->argv, envp) == -1)
		{
			log_error("Error happened in execve during execute_single_command",
				"execve");
			exit(EXIT_FAILURE);	
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if ((WIFEXITED(status) && WEXITSTATUS(status)) || WIFSIGNALED(status))
			return (CMD_FAILURE);
	}
	return (CMD_SUCCESS);
}

int	execute_single_command(t_cmd *cmd, char **envp, int last_status)
{	
	if (is_builtin(cmd->argv[0]))
	{
		if (execute_builtin(cmd, &envp, last_status) == -1)
		{
			log_error("Error happened in execute_builtin during "
				"execute_single_command", "execute_builtin");
			return (CMD_FAILURE);	
		}
		return (CMD_SUCCESS);	
	}
	return (execute_non_builtin_cmd(cmd, envp));
}