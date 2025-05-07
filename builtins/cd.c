/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:14:40 by thchau            #+#    #+#             */
/*   Updated: 2025/05/07 19:42:47 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	cd_builtin(t_cmd *cmd)
{
	char	*path;
	
	if (cmd->argv[1] == NULL)
		path = getenv("HOME");
	else
		path = cmd->argv[1];
	if (chdir(path) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
