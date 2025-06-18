/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_heredoc_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 06:26:31 by thchau            #+#    #+#             */
/*   Updated: 2025/06/18 08:31:38 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

static int	process_heredoc_in_group(t_ast *node, int last_status, char **envp)
{
	int		cur_fd;
	t_redir	*cur;

	g_heredoc_interrupted = 0;
	cur = node->redirs;
	signal(SIGINT, SIG_IGN);
	while (cur)
	{
		if (cur->type == REDIR_HEREDOC)
		{
			cur_fd = process_single_heredoc(cur, last_status, envp);
			if (cur_fd == -1)
			{
				safe_close_fd(node->heredoc_fd);
				return (CMD_FAILURE);
			}
			node->heredoc_fd = cur_fd;
		}
		cur = cur->next;
	}
	signal(SIGINT, sigint_handler);
	return (CMD_SUCCESS);
}

int	preprocess_heredocs_bonus(t_ast *node, int last_status, char **envp)
{
	int		status;

	if (!node)
		return (CMD_SUCCESS);
	status = CMD_SUCCESS;
	if (node->type == NODE_CMD && node->cmd && node->cmd->redirs)
	{
		ft_printf("process heredoc for cmd %s\n", node->cmd->argv[0]);
		status = process_heredoc(node->cmd, last_status, envp);
		ft_printf("cmd->heredoc_id %i\n", node->cmd->heredoc_fd);
	}
	else if (node->type == NODE_GROUP && node->redirs)
	{
		ft_printf("process heredoc for group ....\n");
		status = process_heredoc_in_group(node, last_status, envp);
		ft_printf("group->heredoc_id %i\n", node->heredoc_fd);
	}
		
	if (preprocess_heredocs_bonus(node->left, last_status, envp) != CMD_SUCCESS)
		return (CMD_FAILURE);
	if (preprocess_heredocs_bonus(node->right, last_status, envp) != CMD_SUCCESS)
		return (CMD_FAILURE);
	return (status);
}
