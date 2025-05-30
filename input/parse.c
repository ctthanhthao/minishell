/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:36:53 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/29 12:13:04 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//IDENTIFY TOKEN
t_token	identify_tok(char *str)
{
	if (ft_strncmp(str, "||", 2) == 0)
		return (PIPE);
	else if (ft_strncmp(str, ">>", 2) == 0)
		return (REDIR_OUT);
	else if (ft_strncmp(str, ">", 1) == 0)
		return (REDIR_OUT_APPEND);
	else if (ft_strncmp(str, "<<", 2) == 0)
		return (REDIR_IN);
	else if (ft_strncmp(str, "<", 1) == 0)
		return (REDIR_HEREDOC);
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

// static void	clean_up(char **tokens, t_cmd *head, t_cmd *curr, t_cmd *prev)
// {
// 	free_split(tokens);
// 	free_cmd(head);
// 	if (curr && curr != prev) // not yet linked to head
// 	{
// 		if (curr->argv)
// 			free(curr->argv);
// 		free(curr);
// 	}
// }

void	initialize_state(t_parse_state *s, int argv_i,
		int last_status, char **envp)
{
	s->i = 0;
	s->argv_i = argv_i;
	s->curr = NULL;
	s->head = NULL;
	s->prev = NULL;
	s->last_status = last_status;
	s->envp = envp;
}

t_cmd	*token_loop(char **tokens, int argv_i, int last_status, char **envp)
{
	t_parse_state	s;
	int				ret;

	ret = 0;
	initialize_state(&s, argv_i, last_status, envp);
	while (tokens[s.i])
	{
		ret = handle_token(tokens, &s);
		if (ret == -1)
			return (free_split(tokens), free_cmd(s.head), NULL);
		if (ret == 2)
			continue ;
	}
	if (s.curr)
		s.curr->argv[s.argv_i] = NULL;
	s.prev = s.curr;
	return (s.head);
}

t_cmd	*parse_input(char *input, int last_status, char **envp)
{
	char	**tokens;
	int		argv_i;
	t_cmd	*head;

	if (!check_unclosed_quotes(input) || !check_invalid_chars(input))
		return (NULL);
	tokens = ft_tokenize(input);
	if (!validate_tokens(tokens) || !tokens || !wildcard_check(tokens))
		return (free_split(tokens), NULL);
	argv_i = 0;
	head = token_loop(tokens, argv_i, last_status, envp);
	if (!head)
		return (NULL);
	free_split(tokens);
	return (head);
}

//DEBUG FUNCTION TO PRINT TOKENS:

// void print_cmds(t_cmd *cmd)
// {
//     const char  *token_names[] = {
//         "CMD", "WORD", "PIPE", "MORE", "MOREMORE", "LESS", "LESSLESS",
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
