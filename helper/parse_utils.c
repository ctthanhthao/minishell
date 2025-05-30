/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 12:42:11 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/30 11:34:21 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clean_up(char **tokens, t_cmd *head, t_cmd *curr, t_cmd *prev)
{
	free_split(tokens);
	free_cmd(head);
	if (curr && curr != prev)
	{
		if (curr->argv)
			free(curr->argv);
		free(curr);
	}
}

int	init_curr(t_cmd **curr, int *argv_i, t_cmd **head, t_cmd **prev)
{
	if (!*curr)
	{
		*curr = malloc(sizeof(t_cmd));
		if (!*curr)
			return (0);
		(*curr)->argv = malloc(sizeof(char *) * 1024);
		if (!(*curr)->argv)
			return (0);
		(*curr)->redirs = NULL;
		(*curr)->next = NULL;
		(*curr)->next_type = CMD_NONE;
		*argv_i = 0;
		if (!*head)
			*head = *curr;
		else
			(*prev)->next = *curr;
	}
	return (1);
}

void	token_checker(t_cmd *curr, char *c)
{
	if (curr)
	{
		if (ft_strcmp(c, "||") == 0)
			curr->next_type = CMD_OR_IF;
		else if (ft_strcmp(c, "|") == 0)
			curr->next_type = CMD_PIPE;
		else if (ft_strcmp(c, "&&") == 0)
			curr->next_type = CMD_AND_IF;
	}
}

void	token_init(t_cmd **curr, t_cmd **prev, int *i, int argv_i)
{
	(*curr)->argv[argv_i] = NULL;
	(*i)++;
	*prev = *curr;
	*curr = NULL;
}

int	handle_token(char **tokens, t_parse_state *s)
{
	char	**expanded;
	int		j;

	if (!init_curr(&s->curr, &s->argv_i, &s->head, &s->prev))
		return (clean_up(tokens, s->head, s->curr, s->prev), 0);
	if (is_logical_op(tokens[s->i]))
		return (token_checker(s->curr, tokens[s->i]),
			token_init(&s->curr, &s->prev, &s->i, s->argv_i), 2);
	if (is_redirection(tokens[s->i]) && !is_quoted(tokens[s->i]))
	{
		if (!handle_redirection(s->curr, tokens, &s->i))
			return (clean_up(tokens, s->head, s->curr, s->prev), -1);
		return (2);
	}
	if (s->i == 0)
		expanded = handle_expansion_if_any(tokens[s->i++], s->last_status,
					s->envp, true);
	else
		expanded = handle_expansion_if_any(tokens[s->i++], s->last_status,
					s->envp, false);
	if (expanded)
	{
		j = 0;
		while (expanded[j] && expanded[j][0] != '\0')
			s->curr->argv[s->argv_i++] = expanded[j++];
		free(expanded);
	}
	return (1);
}
