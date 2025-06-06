/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:38:06 by amarcz            #+#    #+#             */
/*   Updated: 2025/06/06 11:06:12 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

t_ast *new_ast_node(t_node_type type, t_ast *left, t_ast *right, t_cmd *cmd)
{
	t_ast *node = malloc(sizeof(t_ast));
	if (!node)
		return NULL;
	node->type = type;
	node->left = left;
	node->right = right;
	node->cmd = cmd;
	node->redirs = NULL;
	return node;
}

char	**safe_realloc(char **argv, int capacity)
{
	char **new_argv;
	
	capacity *= 2;
	new_argv = realloc(argv, sizeof(char *) * capacity);
	if (!new_argv)
	{
		free_split(argv);
		return (NULL);
	}
	return (new_argv);
}
