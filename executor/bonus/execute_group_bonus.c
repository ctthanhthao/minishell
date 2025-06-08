/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_group_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:12:27 by thchau            #+#    #+#             */
/*   Updated: 2025/06/06 19:12:26 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

int	execute_group(t_ast *node, int *last_status, char ***envp)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
		return (log_errno(NULL), CMD_FAILURE);
	if (pid == 0)
	{
		if (apply_redirections(node->redirs, *last_status, *envp) == CMD_FAILURE)
			exit(CMD_FAILURE);
		exit(execute_ast(node->left, last_status, envp));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (128 + WTERMSIG(status));
}