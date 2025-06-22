/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:17:35 by thchau            #+#    #+#             */
/*   Updated: 2025/06/22 22:32:35 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

 int	execute_child_with_redir(int *fds, int from_fd, int to_fd)
{
	if (from_fd != to_fd)
	{
		if (safe_dup2(from_fd, to_fd,
				"dup2 error: bad source fd (-1)\n") == CMD_FAILURE)
		{
			safe_close_fds(fds);
			return (CMD_FAILURE);
		}
	}
	safe_close_fds(fds);
	return (CMD_SUCCESS);
}

// static int	wait_then_get_exit_code(pid_t lpid, pid_t rpid)
// {
// 	int	status;

// 	waitpid(lpid, NULL, 0);
// 	waitpid(rpid, &status, 0);
// 	if (WIFEXITED(status))
// 		return (WEXITSTATUS(status));
// 	return (128 + WTERMSIG(status));
// }

int execute_pipe(t_ast *node, int last_status, char ***envp)
{
    int     pipe_fd[2];
    int     prev_fd = -1;
    pid_t   pid;
    pid_t   last_pid = -1;
    t_ast  *cur = node;
    t_ast  *left;

	while (cur)
	{
		if (cur->type == NODE_PIPE)
			left = cur->left;
		else
			left = cur;
		// ft_printf("[DEBUG] [%i] parent process node = %p node_type = %s\n", getpid(), left, (left) ? node_type_str(left->type) : "NONE");
		if ((cur->right || left->type == NODE_GROUP) && pipe(pipe_fd) == -1)
			return (perror("pipe"), 1);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
		{
//			ft_printf("[DEBUG] [%i] right node = %p node_type = %s\n", getpid(), cur->right, (cur->right) ? node_type_str(cur->right->type) : "NONE");
			signal(SIGPIPE, SIG_DFL);
			if (cur->right)
				dup2(pipe_fd[1], STDOUT_FILENO);
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
			}

			// Close unused pipe ends in child
			if (cur->right || left->type == NODE_GROUP)
			{
				safe_close_fd(&pipe_fd[0]);
				safe_close_fd(&pipe_fd[1]);
			}
			if (prev_fd != -1)
				safe_close_fd(&prev_fd);

			if (left->type == NODE_PIPE)
			{
//				ft_printf("[DEBUG] [%i] child process will execute_pipe\n", getpid());
				exit(execute_pipe(left, last_status, envp));
			}
				
			else if (left->type == NODE_CMD)
			{
				if ((!left->cmd->argv || !*left->cmd->argv) && left->cmd->redirs)
				{
					int status;
					status = handle_builtin_with_redirection(left->cmd, envp, &last_status, NULL);
					exit(status);
				}
				else
//				ft_printf("[DEBUG] [%i] child process execute command %s and prev_fd %i\n", getpid(), left->cmd->argv[0], prev_fd);
					exit(execute_single_command(left->cmd, envp, &last_status, false));
			}
			else if (left->type == NODE_GROUP)
			{
				exit(execute_group(left, last_status, envp));
			}
			else
			{
				fprintf(stderr, "minishell: invalid node in pipeline\n");
				exit(1);
			}
		}
		else
		{
			
			// Parent process closes pipe ends no longer needed
			if (cur->right)
				safe_close_fd(&pipe_fd[1]);
			if (prev_fd != -1)
				safe_close_fd(&prev_fd);
			prev_fd = (cur->right) ? pipe_fd[0] : -1;
//			ft_printf("[DEBUG] [%i] parent process keeps prev_fd %i\n", getpid(), prev_fd);
//			read_from_fd(prev_fd);
		 	last_pid = pid;
		}
		cur = cur->right;
	}
	if (prev_fd != -1)
		safe_close_fd(&prev_fd);
	// Wait for all children and get last child's exit status
	int status;
	waitpid(last_pid, &status, 0);
	return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}
