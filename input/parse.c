/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:36:53 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/21 10:16:45 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//IDENTIFY TOKEN
t_token	identify_tok(char *str)
{
	if (ft_strncmp(str, "||", 2) == 0)
		return (PIPE);
	else if (ft_strncmp(str, ">>", 2) == 0)
		return (MORE);
	else if (ft_strncmp(str, ">", 1) == 0)
		return (MOREMORE);
	else if (ft_strncmp(str, "<<", 2) == 0)
		return (LESS);
	else if (ft_strncmp(str, "<", 1) == 0)
		return (LESSLESS);
	else if (ft_strncmp(str, "&&", 2) == 0)
		return (AND);
	else if (ft_strncmp(str, "|", 1) == 0)
		return (OR);
	return (WORD);
}

//Add a new token node to the list
t_redir	*add_token(t_redir *last, char *value)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->filename = ft_strdup(value);
	new->type = identify_tok(value);
	new->next = NULL;
	if (last)
		last->next = new;
	return (new);
}

//Main parsing function:
t_cmd	*parse_input(char *input, int last_status, char **envp)
{
	char	**tokens;
	int		i;
	int		argv_i;
	t_cmd	*head;
	t_cmd	*curr;
	t_cmd	*prev;

	tokens = ft_tokenize(input);
	if (!validate_tokens(tokens) || !tokens)
	{
		free_split(tokens);
		return (NULL);
	}
	i = 0;
	argv_i = 0;
	head = NULL;
	curr = NULL;
	prev = NULL;
	while (tokens[i])
	{
		if (!curr)
		{
			curr = malloc(sizeof(t_cmd));
			if (!curr)
				return (NULL);
			curr->argv = malloc(sizeof(char *) * 1024);
			curr->redirs = NULL;
			curr->next = NULL;
			curr->next_type = CMD_NONE;
			argv_i = 0;
			if (!head)
				head = curr;
			else
				prev->next = curr;
		}
		if (ft_strncmp(tokens[i], "||", 2) == 0)
		{
			if (curr)
				curr->next_type = CMD_OR_IF;
			curr->argv[argv_i] = NULL;
			i++;
			prev = curr;
			curr = NULL;
			continue ;
		}
		else if (ft_strncmp(tokens[i], "|", 1) == 0)
		{
			if (curr)
				curr->next_type = CMD_PIPE;
			curr->argv[argv_i] = NULL;
			i++;
			prev = curr;
			curr = NULL;
			continue ;
		}
		else if (ft_strncmp(tokens[i], "&&", 2) == 0)
		{
			if (curr)
				curr->next_type = CMD_AND_IF;
			curr->argv[argv_i] = NULL;
			i++;
			prev = curr;
			curr = NULL;
			continue ;
		}
		if (is_redirection(tokens[i]))
		{
			if (!handle_redirection(curr, tokens, &i))
			{
				free_split(tokens);
				free_cmd(head);
				return (NULL);
			}
			continue ;
		}
		curr->argv[argv_i++] = expand_dollar(tokens[i++], last_status, envp);
	}
	if (curr)
		curr->argv[argv_i] = NULL;
	prev = curr;
	free_split(tokens);
	return (head);
}

//DEBUG FUNCTION TO PRINT TOKENS:

// void print_cmds(t_cmd *cmd)
// {
//     const char  *token_names[] = {
//         "CMD", "WORD", "PIPE", "MORE", "MOREMORE", "LESS", "LESSLESS", \
//         "AND", "OR"
//     };
//     const char *cmd_type_strs[] = {"NONE", "AND_IF", "OR_IF", "PIPE"};
//     int i;
//     t_redir *r;

//     i = -1;
//     while (cmd)
//     {
//         ft_printf("==== Command === \n");

//         if (cmd->argv)
//         {
//             while (cmd->argv[++i])
//             {
//                 ft_printf("Arg[%d]: %s\n", i, cmd->argv[i]);
//             }
//             ft_printf("Next type value: %d\n", cmd->next_type);
//             ft_printf("Next type: %s\n", cmd_type_strs[cmd->next_type]);
//         }
//         r = cmd ->redirs;
//         while (r)
//         {
//             ft_printf("Redir: %-10s | Type: %s\n", r->filename,
//             		token_names[r->type]);
//             r = r->next;
//         }
//         cmd = cmd->next;
//         i = -1;
//     }
// }
