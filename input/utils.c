/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:38:06 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/07 20:22:30 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_split(char **arr)
{
    int i;

    i = 0;
    if (!arr)
        return;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void free_cmd(t_cmd *cmd)
{
	int i;
	t_redir *tmp;

	if (!cmd) return;
	i = 0;
	while (cmd->argv[i])
	{
		free(cmd->argv[i]);
		i++;
	}
	while (cmd->redirs)
	{
		tmp = cmd->redirs;
		cmd->redirs = cmd->redirs->next;
		free(tmp->filename);
		free(tmp);
	}
	free(cmd->argv);
	free(cmd);
}

void	free_envp(char **envp)
{
	int i;

	if (!envp)
		return;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}