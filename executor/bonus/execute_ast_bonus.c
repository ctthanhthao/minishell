/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 20:17:14 by thchau            #+#    #+#             */
/*   Updated: 2025/06/05 19:08:56 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

int	execute_ast(t_ast *node)
{
	int left_status;
	
	if (!node)
		return (1);
	if (node->type == NODE_CMD)
		return (exec_cmd(node->cmd));
	else if (node->type == NODE_PIPE)
		return (exec_pipe(node->left, node->right));
	else if (node->type == NODE_AND)
	{
		left_status = execute_ast(node->left);
		if (execute_ast(node->left) == 0)
			return execute_ast(node->right);
		return left_status;
	}
	else if (node->type == NODE_OR)
	{
		left_status = execute_ast(node->left);
		if (left_status != 0)
			return execute_ast(node->right);
		return left_status;
	}
	else if (node->type == NODE_GROUP)
	{
		// subshell or scoped execution
		return exec_group(node->left);
	}
	return 1;
}