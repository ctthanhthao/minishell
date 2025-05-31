/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:07:20 by thchau            #+#    #+#             */
/*   Updated: 2025/05/31 17:11:33 by thchau           ###   ########.fr       */
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
	t_cmd	*next;
	t_redir	*tmp_re;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->argv)
		{
			i = 0;
			while (cmd->argv[i])
				free(cmd->argv[i++]);
			free(cmd->argv);
		}
		while (cmd->redirs)
		{
			tmp_re = cmd->redirs;
			cmd->redirs = cmd->redirs->next;
			if (tmp_re->filename)
				free(tmp_re->filename);
			free(tmp_re);
		}
		free(cmd);
		cmd = next;
	}
}
