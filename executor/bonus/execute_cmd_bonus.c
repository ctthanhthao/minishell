/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:02:21 by thchau            #+#    #+#             */
/*   Updated: 2025/06/08 08:25:24 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

int	execute_cmd(t_cmd *cmd, int *last_status, char ***envp)
{
	if ((!cmd->argv || !*cmd->argv) && cmd->redirs)
		return(handle_builtin_with_redirection(cmd, envp, last_status, NULL));
	return (execute_single_command(cmd, envp, last_status, true));
}
