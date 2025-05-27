/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:36:53 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/27 12:42:43 by amarcz           ###   ########.fr       */
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

int are_full_quotes(const char *s)
{
	int len;

	len = ft_strlen(s);
	if (len < 2)
		return (0);
	if ((s[0] == '\'' && s[len - 1] == '\'') || (s[0] == '"' && s[len - 1] == '"'))
		return (1);
	return (0);
}

int wildcard_check(char **tokens)
{
	int i;

	i = 0;
	while (tokens[i])
	{
		if (ft_strchr(tokens[i], '*'))
		{
			if (ft_strchr(tokens[i], '\'') || ft_strchr(tokens[i], '\"'))
			{
				if(!are_full_quotes(tokens[i]))
				{
					ft_printf(R "Dude, WHAT IS THIS?!! ");
					ft_printf("You can't handle wildcards mixed with quotes like this: %s\n" RST, tokens[i]);
					return (0);
				}
			}
		}
		i++;
	}
	return (1);
}


int check_unclosed_quotes(const char *input)
{
	int i;
	int single;
	int dquote;

	i = 0;
	single = 0;
	dquote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && dquote % 2 == 0)
			single++;
		else if (input[i] == '\"' && single % 2 == 0)
			dquote++;
		i++;
	}
	if (single % 2 != 0 || dquote % 2 != 0)
	{
		if (single % 2 != 0)
			return (ft_printf(R "Whoa, you messed up! You forgot to close a single quote, Dudio!\n" RST), 0);
		else
			return (ft_printf(R "Whoa! You forgot to close a double quote! Pay attention, Bro!\n" RST), 0);
	}
	return (1);
}

//Main parsing function:
t_cmd	*parse_input(char *input, int last_status, char **envp)
{
	char	**tokens;
	// int		i;
	// int		j;
	int		argv_i;
	t_cmd	*head;
	t_cmd	*curr;
	t_cmd	*prev;
	// char 	**expanded;

	if (!check_unclosed_quotes(input))
		return (NULL);
	tokens = ft_tokenize(input);
	if (!validate_tokens(tokens) || !tokens || !wildcard_check(tokens))
		return (free_split(tokens), NULL);
	argv_i = 0;
	curr = NULL;
	prev = NULL;
	// while (tokens[i])
	// {
	// 	if (!curr)
	// 	{
	// 		curr = malloc(sizeof(t_cmd));
	// 		if (!curr)
	// 		{
	// 			clean_up(tokens, head, curr, prev);
	// 			return (NULL);
	// 		}
	// 		curr->argv = malloc(sizeof(char *) * 1024);
	// 		if (!curr->argv)
	// 		{
	// 			clean_up(tokens, head, curr, prev);
	// 			return (NULL);
	// 		}
	// 		curr->redirs = NULL;
	// 		curr->next = NULL;
	// 		curr->next_type = CMD_NONE;
	// 		argv_i = 0;
	// 		if (!head)
	// 			head = curr;
	// 		else
	// 			prev->next = curr;
	// 	}
	// 	if (ft_strncmp(tokens[i], "||", 2) == 0)
	// 	{
	// 		if (curr)
	// 			curr->next_type = CMD_OR_IF;
	// 		curr->argv[argv_i] = NULL;
	// 		i++;
	// 		prev = curr;
	// 		curr = NULL;
	// 		continue ;
	// 	}
	// 	else if (ft_strncmp(tokens[i], "|", 1) == 0)
	// 	{
	// 		if (curr)
	// 			curr->next_type = CMD_PIPE;
	// 		curr->argv[argv_i] = NULL;
	// 		i++;
	// 		prev = curr;
	// 		curr = NULL;
	// 		continue ;
	// 	}
	// 	else if (ft_strncmp(tokens[i], "&&", 2) == 0)
	// 	{
	// 		if (curr)
	// 			curr->next_type = CMD_AND_IF;
	// 		curr->argv[argv_i] = NULL;
	// 		i++;
	// 		prev = curr;
	// 		curr = NULL;
	// 		continue ;
	// 	}
	// 	if (is_redirection(tokens[i]))
	// 	{
	// 		if (!handle_redirection(curr, tokens, &i))
	// 		{
	// 			clean_up(tokens, head, curr, prev);
	// 			return (NULL);
	// 		}
	// 		continue ;
	// 	}
	// 	expanded = handle_expansion_if_any(tokens[i++], last_status, envp);
	// 	if (expanded)
	// 	{
	// 		j = 0;
	// 		while (expanded[j])
	// 			curr->argv[argv_i++] = expanded[j++];
	// 		free(expanded); // Free the wrapper, not the strings
	// 	}
	// }
	// if (curr)
	// 	curr->argv[argv_i] = NULL;
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
