/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 11:32:50 by thchau            #+#    #+#             */
/*   Updated: 2025/05/29 12:08:37 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	process_write(t_redir *re, int type)
{
	int	fd;

	if (type == REDIR_OUT)
	{
		fd = open(re->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		return (safe_dup2(fd, STDOUT_FILENO, NULL));
	}
	if (type == REDIR_OUT_APPEND)
	{
		fd = open(re->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		return (safe_dup2(fd, STDOUT_FILENO, NULL));
	}
	return (CMD_FAILURE);
}

static int	process_read(t_redir *re, int type)
{
	int	fd;
	char	*file;

	if (type == REDIR_IN)
	{
		file = ft_strtrim(re->filename, "\"\'");
		fd = open(file, O_RDONLY);
		free(file);
		return (safe_dup2(fd, STDIN_FILENO, NULL));
	}
	if (type == REDIR_HEREDOC)
		return (process_heredoc(re));
	return (CMD_FAILURE);
}

int	apply_redirections(t_redir *redir_list)
{
	t_redir	*cur;
	int		status;

	status = CMD_SUCCESS;
	if (redir_list == NULL)
		return (status);
	cur = redir_list;
	while (cur)
	{
		if (redir_list->filename == NULL)
			return (CMD_FAILURE);
		if (cur->type == REDIR_OUT || cur->type == REDIR_OUT_APPEND)
			status = process_write(cur, cur->type);
		else if (cur->type == REDIR_IN || cur->type == REDIR_HEREDOC)
		{
			status = process_read(cur, cur->type);
			while (cur->next && cur->next->type == REDIR_HEREDOC)
				cur = cur->next;
		}
		if (status == CMD_FAILURE)
			break ;
		cur = cur->next;
	}
	return (status);
}
