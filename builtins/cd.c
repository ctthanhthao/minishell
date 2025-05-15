/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:14:40 by thchau            #+#    #+#             */
/*   Updated: 2025/05/15 11:03:21 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	cd_builtin(t_cmd *cmd)
{
	char	*path;

	ft_printf("cd builtin is called...\n");
	if (cmd->argv[1] == NULL)
		path = getenv("HOME");
	else if (cmd->argv[1][0] == '$')
		path = getenv(cmd->argv[1] + 1);
	else
		path = cmd->argv[1];
	if (chdir(path) == -1)
	{
		perror("cd");
		return (127);
	}
	return (CMD_SUCCESS);
}
