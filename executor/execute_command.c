/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:42:06 by thchau            #+#    #+#             */
/*   Updated: 2025/05/15 11:23:33 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		execute_commands(t_cmd *cmd_list, char **envp)
{
	t_cmd	*current_cmd;
	int		pipe_fd[2];
	pid_t	pid;
	
	if (cmd_list)
		return (0);
	current_cmd = cmd_list;
	while (current_cmd)
	{
		if (current_cmd->next_type == CMD_PIPE)
		{
			process_pipe_in_cmd(pipe_fd, pid, current_cmd, envp);
			while (current_cmd->next && current_cmd->next_type == CMD_PIPE)
				current_cmd = current_cmd->next;
		}
		else
		{
			if (fork() == 0)
			{
				if (is_builtin(current_cmd->argv[0]))
				{
					if (execute_builtin(current_cmd, &envp, 0) == -1)
						log_error("Error happened in execute_builtin during "
							"processing pipe", "execute_builtin");
					exit(EXIT_SUCCESS);
				}
				execve(current_cmd->argv[0], current_cmd->argv, envp);
				perror("execve");
				exit(EXIT_FAILURE);
			}
			wait(NULL);
		}
		current_cmd = current_cmd->next;	
	}
}