/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:14:40 by thchau            #+#    #+#             */
/*   Updated: 2025/05/30 12:36:57 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	cd_builtin(t_cmd *cmd)
{
	char	*path;

	if (cmd->argv[1] == NULL)
		path = getenv("HOME");
	else if (cmd->argv[2])
	{
		log_errno("too many arguments");
		return (CMD_FAILURE);
	}
	else
		path = cmd->argv[1];
	if (chdir(path) != 0)
	{
		perror("cd");
		return (126);
	}
	return (CMD_SUCCESS);
}
