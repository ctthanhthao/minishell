/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:18:30 by thchau            #+#    #+#             */
/*   Updated: 2025/06/06 12:10:17 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

static const char	*node_type_str(t_node_type type)
{
	if (type == NODE_CMD)
		return ("CMD");
	else if (type == NODE_PIPE)
		return ("PIPE - |");
	else if (type == NODE_AND)
		return ("AND - &&");
	else if (type == NODE_OR)
		return ("OR - ||");
	else if (type == NODE_GROUP)
		return ("GROUP - ()");
	else
		return ("UNKNOWN");
}

static const char	*node_color(t_node_type type)
{
	if (type == NODE_CMD)
		return (COLOR_CMD);
	else if (type == NODE_PIPE)
		return (COLOR_OP);
	else if (type == NODE_AND)
		return (COLOR_OP);
	else if (type == NODE_OR)
		return (COLOR_OP);
	else if (type == NODE_GROUP)
		return (COLOR_GROUP);
	else
		return (COLOR_RESET);
}

static void	prepare_prefix(const char *prefix, int is_last, char *new_prefix)
{
	ft_strlcpy(new_prefix, (char *)prefix, 2048);
	if (is_last)
		ft_strlcat(new_prefix, "    ", 2048);
	else
		ft_strlcat(new_prefix, "|   ", 2048);
}

static void	print_ast_node(t_ast *node, const char *prefix, int is_last)
{
	int		i;
	char	new_prefix[2048];

    if (!node)
		return;
	if (is_last)
    	ft_printf("%s└── ", prefix);
    else
		ft_printf("%s├── ", prefix);
	ft_printf("%s%s", node_color(node->type), node_type_str(node->type));
	if (node->type == NODE_CMD && node->cmd && node->cmd->argv)
    {
        ft_printf(": ");
		i = 0;
		while (node->cmd->argv[i])
			ft_printf("%s ", node->cmd->argv[i++]);
    }
    ft_printf(COLOR_RESET"\n");
	prepare_prefix(prefix, is_last, new_prefix);
	print_redirections(node, prefix);
	if (node->left)
		print_ast_node(node->left, new_prefix, !node->right);
	if (node->right)
		print_ast_node(node->right, new_prefix, 1);
}

void print_ast(t_ast *root)
{
    print_ast_node(root, "", 1);
}