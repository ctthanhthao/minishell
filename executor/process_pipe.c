/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:40:57 by thchau            #+#    #+#             */
/*   Updated: 2025/05/14 14:05:08 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	process_pipe_in_cmd(int *pipe_fd, int pid, t_cmd *cmd, char **envp)
{
	if (pid == 0)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			close(pipe_fd[1]);
			log_error("Error happened in dup2 during processing pipe", "dup2");
		}
		close(pipe_fd[1]);
		if (is_builtin(cmd->argv[0]))
		{
			if (execute_builtin(cmd, &envp, 0) == -1)
				log_error("Error happened in execute_builtin during "
					"processing pipe", "execute_builtin");
			exit(EXIT_SUCCESS);	
		}
		if (execve(cmd->argv[0], cmd->argv, envp) == -1)
				log_error("Error happened in execve during processing pipe",
					"execve");
	}
	else
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
}

int	process_pipe(t_cmd *cmd, char **envp)
{
	int		status;
	int		pipe_fd[2];
	t_cmd	*cur;
	int		pid;

	if (!cmd)
		return (0);
	if (cmd->next_type != CMD_PIPE)
		return (-1);
	status = 0;
	cur = cmd;
	while (cur)
	{
		if (pipe(pipe_fd) == -1)
				return (-1);
		pid = fork();
		if (pid == -1)
		{
			log_error("Error happened in fork during processing pipe",
				"fork");
			return (-1);
		}	
		process_pipe_in_cmd(pipe_fd, pid, cur, envp);
//		current_cmd = current_cmd->next;
	}
	return (0);
}