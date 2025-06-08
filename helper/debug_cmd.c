/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 15:01:15 by thchau            #+#    #+#             */
/*   Updated: 2025/06/07 15:02:54 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

const char *token_to_str(t_token type)
{
	switch (type)
	{
		case REDIR_IN:   return "<";
		case REDIR_OUT:  return ">";
		case REDIR_OUT_APPEND: return ">>";
		case REDIR_HEREDOC:  return "<<";
		default:         return "UNKNOWN";
	}
}

const char *cmd_type_to_str(t_cmd_type type)
{
	switch (type)
	{
		case CMD_NONE:   return "NONE";
		case CMD_AND_IF: return "AND_IF";
		case CMD_OR_IF:  return "OR_IF";
		case CMD_PIPE:   return "PIPE";
		default:         return "UNKNOWN";
	}
}

void	debug_redirs(t_redir *r)
{
	while (r)
	{
		printf("    Redirection: type=%s, filename='%s'\n",
			token_to_str(r->type), r->filename);
		r = r->next;
	}
}

void	debug_cmd(t_cmd *cmd)
{
	int i;

	while (cmd)
	{
		printf("Command:\n");

		if (cmd->argv)
		{
			for (i = 0; cmd->argv[i]; i++)
				printf("  argv[%d] = '%s'\n", i, cmd->argv[i]);
		}
		else
		{
			printf("  argv = (null)\n");
		}

		if (cmd->redirs)
			debug_redirs(cmd->redirs);
		else
			printf("  No redirections\n");

		printf("  next_type = %s\n", cmd_type_to_str(cmd->next_type));
		cmd = cmd->next;
		printf("\n");
	}
}