/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:42:06 by thchau            #+#    #+#             */
/*   Updated: 2025/05/28 22:42:17 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_commands(t_cmd *cmd_list, char ***envp, int *last_status)
{
	t_cmd	*cur;
	bool	redirected;
	int		stdin_bk;
	int		stdout_bk;

	if (!cmd_list)
		return (CMD_SUCCESS);
	cur = cmd_list;
	while (cur)
	{
		if (cur->next_type == CMD_PIPE)
		{
			*last_status = process_pipe(cur, envp, last_status);
			while (cur->next && cur->next_type == CMD_PIPE)
				cur = cur->next;
		}
		else
		{
			stdin_bk = -1;
			stdout_bk = -1;
			if (cur->redirs)
				redirected = save_original_std_inout(&stdin_bk, &stdout_bk);
			if (apply_redirections(cur->redirs) == CMD_FAILURE)
				return (CMD_FAILURE);
			*last_status = execute_single_command(cur, envp, last_status, true);
			if (redirected)
				restore_original_std_inout(stdin_bk, stdout_bk);
		}
		if (cur->next_type == CMD_AND_IF && *last_status != CMD_SUCCESS)
			break ;
		if (cur->next_type == CMD_OR_IF && *last_status == CMD_SUCCESS)
			break ;
		cur = cur->next;
	}
	return (*last_status);
}
