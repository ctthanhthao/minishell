/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_to_ast_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 19:37:50 by thchau            #+#    #+#             */
/*   Updated: 2025/06/08 16:36:23 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

char	**extent_argv_if_need(char **argv, int *capacity, int argc)
{
	char	**new_argv;
	int		new_capacity;

	if (argc + 1 >= *capacity)
	{
		new_capacity = *capacity * 2;
		new_argv = safe_realloc(argv, *capacity, new_capacity);
		if (!new_argv)
			return (NULL);
		*capacity = new_capacity;
		return (new_argv);
	}
	return (argv);
}

static t_cmd *new_cmd(int capacity)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = malloc(sizeof(char *) * capacity);
	if (!cmd->argv)
		return (free(cmd), NULL);
	cmd->redirs = NULL;
	return (cmd);
}

static t_cmd *parse_command(t_parser *p)
{
	t_cmd	*cmd;
	int		capacity;
	int		argc;
	
	capacity = 10;
	cmd = new_cmd(capacity);
	argc = 0;
	while (p->tokens[p->tokeni] && !is_logical_op(p->tokens[p->tokeni])
		&& ft_strcmp(p->tokens[p->tokeni], ")") != 0)
	{
		if (ft_is_redirection(p->tokens[p->tokeni]))
		{
			parse_redirections_bonus(&cmd->redirs, p);
			continue;
		}
		if (handle_expansion_bonus(&cmd->argv, &argc, &capacity, p) == CMD_FAILURE)
			return (free(cmd), NULL);
		p->tokeni++;
	}
	return (cmd->argv[argc] = NULL, cmd);
}

static t_ast	*parse_group_or_command(t_parser *p)
{
	t_redir	*redirs;
	t_ast	*node;
	t_ast	*group;
	t_cmd	*cmd;
	
	node = NULL;
	redirs = NULL;
	if (p->tokeni < p->token_count && ft_strcmp(p->tokens[p->tokeni], "(") == 0)
	{
		p->tokeni++;
		group = parse_expression(p);
		if (p->tokeni < p->token_count && ft_strcmp(p->tokens[p->tokeni], ")") == 0)
			p->tokeni++;
		else
			return (log_errno("Syntax error: expected ')'"), NULL);
		parse_redirections_bonus(&redirs, p);
		node = new_ast_node(NODE_GROUP, group, NULL, NULL);
		return (node->redirs = redirs, node);
	}
	else
	{
		cmd = parse_command(p);
		if (!cmd)
			return (NULL);
		return (new_ast_node(NODE_CMD, NULL, NULL, cmd));
	}
}

/*
* Parse_pipeline handles |
* Higher precedence than && and ||
*/ 
static	t_ast *parse_pipeline(t_parser *p)
{
	t_ast	*left;
	t_ast	*right;
	
	left = parse_group_or_command(p);
	if (!left)
		return (NULL);
	while (p->tokeni < p->token_count
		&& ft_strcmp(p->tokens[p->tokeni], "|") == 0)
	{
		p->tokeni++;
		right = parse_group_or_command(p);
		if (!right)
			return (left);
		left = new_ast_node(NODE_PIPE, left, right, NULL);
	}
	return (left);
}

/**
 * Parse_expression handles && and ||
 * Lowest precedence, left-associative
 */ 
t_ast *parse_expression(t_parser *p)
{
	t_ast 		*left;
	t_ast		*right;
	t_node_type	type;

	left = parse_pipeline(p);
	if (!left)
		return (NULL);
	while (p->tokeni < p->token_count &&
		   (ft_strcmp(p->tokens[p->tokeni], "&&") == 0
		   || ft_strcmp(p->tokens[p->tokeni], "||") == 0))
	{
		type = (ft_strcmp(p->tokens[p->tokeni], "&&") == 0) ? NODE_AND : NODE_OR;
		p->tokeni++;
		right = parse_pipeline(p);
		if (!right)
			return (left);
		left = new_ast_node(type, left, right, NULL);
	}
	return (left);
}
