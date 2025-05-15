/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:40:57 by thchau            #+#    #+#             */
/*   Updated: 2025/05/15 10:56:50 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	pre_execute()
{
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0 ||
		ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0 ||
		ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0 ||
		ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}
static void	execute(int *pipe_fd, t_cmd *cmd, char **envp)
{
	pid_t	pid;
	
	if (pipe(pipe_fd) == -1)
	{
		log_error("Error happened in pipe during processing pipe",
			"pipe");
		return (CMD_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		log_error("Error happened in fork during processing pipe",
			"fork");
		return (CMD_FAILURE);
	}
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

	if (!cmd)
		return (CMD_SUCCESS);
	if (cmd->next_type != CMD_PIPE)
		return (CMD_FAILURE);
	cur = cmd;
	while (cur)
	{
		execute(pipe_fd, cur, envp);
		if (cur->next && cur->next->next_type == CMD_PIPE)
			cur = cur->next;
		else
			break ;
	}
	return (CMD_SUCCESS);
}