/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:07:20 by thchau            #+#    #+#             */
/*   Updated: 2025/05/08 22:00:09 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_split(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
}

void	free_cmd(t_cmd *cmd)
{
	int		i;
	t_redir	*tmp;

	if (!cmd)
		return ;
	ft_printf("free_cmd is called...\n");
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
		tmp->filename = NULL;
		free(tmp);
		tmp = NULL;
	}
	free(cmd->argv);
	free(cmd);
	cmd = NULL;
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	ft_printf("free_envp is called...\n");
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		envp[i] = NULL;
		i++;
	}
}
