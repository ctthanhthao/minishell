/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clone.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:10:16 by thchau            #+#    #+#             */
/*   Updated: 2025/05/31 21:28:43 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	free_created_elements(char **ar, int to_pos)
{
	int	k;

	k = 0;
	while (k < to_pos)
	{
		free(ar[k]);
		ar[k] = NULL;
		k++;
	}
}

char	**clone_arr(char **ar)
{
	int		len;
	int		i;
	char	**copy;

	if (!ar || !*ar)
		return (NULL);
	len = 0;
	i = 0;
	while (ar[len])
		len++;
	copy = malloc(sizeof(char *) * (len + 1));
	if (!copy)
		return (NULL);
	while (ar[i])
	{
		copy[i] = ft_strdup(ar[i]);
		if (!copy[i])
		{
			free_created_elements(copy, i);
			free(copy);
			return (NULL);
		}
		i++;
	}
	return (copy[i] = NULL, copy);
}

static char	*get_file_name(char *name)
{
	char	*file_name;

	if (name)
		file_name = ft_strdup(name);
	else
		file_name = NULL;
	return (file_name);
}

static t_redir	*clone_redirs(t_redir *redirs)
{
	t_redir	*new_redirs;
	t_redir	*last;
	t_redir	*new_node;

	last = NULL;
	new_redirs = NULL;
	while (redirs)
	{
		new_node = malloc(sizeof(t_redir));
		if (!new_node)
			return (NULL);
		new_node->type = redirs->type;
		new_node->filename = get_file_name(redirs->filename);
		if (redirs->filename && !new_node->filename)
			return (free(new_node), NULL);
		new_node->next = NULL;
		if (last)
			last->next = new_node;
		else
			new_redirs = new_node;
		last = new_node;
		redirs = redirs->next;
	}
	return (new_redirs);
}

t_cmd	*clone_cmd(t_cmd *cmd)
{
	t_cmd	*new_cmd;

	if (!cmd)
		return (NULL);
	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->argv = clone_arr(cmd->argv);
	new_cmd->redirs = clone_redirs(cmd->redirs);
	new_cmd->next = clone_cmd(cmd->next);
	new_cmd->next_type = cmd->next_type;
	return (new_cmd);
}
