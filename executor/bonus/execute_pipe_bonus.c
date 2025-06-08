/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:17:35 by thchau            #+#    #+#             */
/*   Updated: 2025/06/06 17:07:37 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

static int	execute_child_with_redirect(int *fds, int from_fd, int to_fd)
{
	if (safe_dup2(from_fd, to_fd,
				"dup2 error: bad source fd (-1)\n") == CMD_FAILURE)
	{
		close(fds[1]);
		close(fds[0]);
		return (CMD_FAILURE);
	}
	close(fds[1]);
	close(fds[0]);
	return (CMD_SUCCESS);
}

static int	wait_then_get_exit_code(pid_t lpid, pid_t rpid)
{
	int	status;
	
	waitpid(lpid, NULL, 0);
	waitpid(rpid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (128 + WTERMSIG(status));
}

int	execute_pipe(t_ast *left, t_ast *right, int *last_status, char ***envp)
{
	int		fds[2];
	pid_t	lpid;
	pid_t	rpid;
	
	if (pipe(fds) < 0)
		return (log_errno(NULL), CMD_FAILURE);
	lpid = fork();
	if (lpid == 0)
	{
		if (execute_child_with_redirect(fds, fds[1], STDOUT_FILENO)
					== CMD_FAILURE)
			exit(CMD_FAILURE);
		exit(execute_ast(left, last_status, envp));
	}
	rpid = fork();
	if (rpid == 0)
	{
		if (execute_child_with_redirect(fds, fds[0], STDIN_FILENO)
					== CMD_FAILURE)
			exit(CMD_FAILURE);
		exit(execute_ast(right, last_status, envp));
	}
	close(fds[0]);
	close(fds[1]);
	return (wait_then_get_exit_code(lpid, rpid));
}