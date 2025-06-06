/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:40:57 by thchau            #+#    #+#             */
/*   Updated: 2025/06/05 20:53:00 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * Collect status of last command.
 */
static void	collect_pipeline_status(t_pid_pipe_fd *pid_data, int *last_status)
{
	int	i;
	int	status;

	i = -1;
	while (++i < pid_data->child_count)
	{
		if (waitpid(pid_data->pids[i], &status, 0) == -1)
			continue ;
		if (i == pid_data->child_count - 1)
		{
			if (WIFEXITED(status))
				*last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*last_status = 128 + WTERMSIG(status);
			else
				*last_status = CMD_FAILURE;
		}
	}
}

static void	execute_pipeline_child(t_cmd *cur, char ***env,
	t_pid_pipe_fd *pid_data, int *last_status)
{
	if (pid_data->prev_fd != -1)
	{
		if (safe_dup2(pid_data->prev_fd, STDIN_FILENO,
				"dup2 error: bad source fd (-1)\n") == CMD_FAILURE)
		{
			close(pid_data->pipe_fd[0]);
			close(pid_data->pipe_fd[1]);
			exit (CMD_FAILURE);
		}
	}
	if (cur->next_type == CMD_PIPE)
	{
		if (safe_dup2(pid_data->pipe_fd[1], STDOUT_FILENO,
				"dup2 error: bad source fd (-1)\n") == CMD_FAILURE)
		{
			close(pid_data->pipe_fd[1]);
			close(pid_data->pipe_fd[0]);
			exit(CMD_FAILURE);
		}
		close(pid_data->pipe_fd[1]);
		close(pid_data->pipe_fd[0]);
	}
	if (pid_data->pipe_fd[0] != -1)
		close(pid_data->pipe_fd[0]);
	*last_status = execute_single_command(cur, env, last_status, false);
	exit(*last_status);
}

static int	create_pipeline_if_needed(t_cmd *cur, t_pid_pipe_fd *pid_data)
{
	if (cur->next && cur->next_type == CMD_PIPE)
	{
		if (pipe(pid_data->pipe_fd) == -1)
			return (log_errno("Pipe failed in process_pipe"), CMD_FAILURE);
	}
	else
	{
		pid_data->pipe_fd[0] = -1;
		pid_data->pipe_fd[1] = -1;
	}
	return (CMD_SUCCESS);
}

static int	spawn_pipeline_process(t_pid_pipe_fd *pid_data, t_cmd *cur,
	char ***envp, int *last_status)
{
	pid_data->pid = fork();
	if (pid_data->pid == -1)
		return (log_errno("Fork failed in process_pipe"), CMD_FAILURE);
	if (pid_data->pid == 0)
		execute_pipeline_child(cur, envp, pid_data, last_status);
	else
	{
		pid_data->pids[pid_data->child_count++] = pid_data->pid;
		if (pid_data->prev_fd != -1)
			close(pid_data->prev_fd);
		if (pid_data->pipe_fd[1] != -1)
			close(pid_data->pipe_fd[1]);
		pid_data->prev_fd = pid_data->pipe_fd[0];
	}
	return (CMD_SUCCESS);
}

int	process_pipe(t_cmd *cmd, char ***envp, int *last_status)
{
	t_cmd			*cur;
	t_pid_pipe_fd	pid_data;

	cur = cmd;
	pid_data.prev_fd = -1;
	pid_data.child_count = 0;
	while (cur)
	{
		if (create_pipeline_if_needed(cur, &pid_data) == CMD_FAILURE)
			return (CMD_FAILURE);
		if (spawn_pipeline_process(&pid_data, cur, envp, last_status)
			== CMD_FAILURE)
			return (CMD_FAILURE);
		if (cur->next_type == CMD_PIPE)
			cur = cur->next;
		else
			break ;
	}
	collect_pipeline_status(&pid_data, last_status);
	if (pid_data.prev_fd != -1)
		close(pid_data.prev_fd);
	return (*last_status);
}
