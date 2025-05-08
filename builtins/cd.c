/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:14:40 by thchau            #+#    #+#             */
/*   Updated: 2025/05/08 22:06:43 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	cd_builtin(t_cmd *cmd)
{
	char	*path;

	ft_printf("cd builtin is called...\n");
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
