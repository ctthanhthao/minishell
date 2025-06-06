/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 19:12:09 by thchau            #+#    #+#             */
/*   Updated: 2025/06/06 10:56:07 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

t_redir *parse_redirections_bonus(t_parser *p)
{
	t_redir *head = NULL;
	t_redir **curr = &head;

	while (p->tokens[p->tokeni])
	{
		t_token type;

		if (strcmp(p->tokens[p->tokeni], ">") == 0)
			type = REDIR_OUT;
		else if (strcmp(p->tokens[p->tokeni], ">>") == 0)
			type = REDIR_OUT_APPEND;
		else if (strcmp(p->tokens[p->tokeni], "<") == 0)
			type = REDIR_IN;
		else if (strcmp(p->tokens[p->tokeni], "<<") == 0)
			type = REDIR_HEREDOC;
		else
			break;

		p->tokeni++;
		if (!p->tokens[p->tokeni])
		{
			log_errno("Syntax error: expected filename after redirection\n");
			// Optional: free any redirs collected so far
			return NULL;
		}

		t_redir *new = malloc(sizeof(t_redir));
		if (!new)
			return NULL;

		new->type = type;
		new->filename = strdup(p->tokens[p->tokeni++]);
		new->next = NULL;

		*curr = new;
		curr = &new->next;
	}

	return head;
}