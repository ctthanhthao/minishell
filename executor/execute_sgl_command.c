/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_sgl_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:47:05 by thchau            #+#    #+#             */
/*   Updated: 2025/05/19 10:05:31 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	execute_external_cmd(t_cmd *cmd, char **envp)
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
				cmd->argv[0]);
			exit(127);	
		}
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (128 + WTERMSIG(status));
}

int	execute_single_command(t_cmd *cmd, char **envp, int *last_status)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (CMD_SUCCESS);
	if (is_builtin(cmd->argv[0]))
		*last_status = execute_builtin(cmd, &envp, last_status);
	else
		*last_status = execute_external_cmd(cmd, envp);
	return (CMD_SUCCESS);
}