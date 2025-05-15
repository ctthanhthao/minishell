/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_directions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 11:32:50 by thchau            #+#    #+#             */
/*   Updated: 2025/05/15 13:49:48 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int process_heredoc(t_redir *redir)
{
	char	*line;
	int		fd[2];
	t_redir	*cur;
	int		last_fd;
	
	last_fd = -1;
	cur = redir;
	while (cur)
	{
		if (cur->type == LESSLESS)
		{
			if (pipe(fd) == -1)
			{
				log_error("Error happened when pipe fd in process_heredoc", "pipe");
				return (CMD_FAILURE);
			}
			while (1)
			{
			line = readline("> ");
			if (!line || !ft_strcmp(line, cur->filename))
			{
				free(line);
				break;
			}
			write(fd[1], line, ft_strlen(line));
			write(fd[1], "\n", 1);
			free(line);	
			}
			close(fd[1]);
			if (last_fd != -1)
				close(last_fd);
			last_fd = fd[0];
		}
		cur = cur->next;
	}
	if (last_fd != -1)
	{
		if (dup2(last_fd, STDIN_FILENO) == -1)
		{
			log_error("Error when duplicating last_fd in process_heredoc",
				"dup2");
				
		}
	}
	while (1)
	{
		line = readline("> ");
		//if (!line || !ft_strcmp(line, delimiter))
		{
			free(line);
			break;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	
}
static int	handle_file(int fd, int std_in_out, char *error)
{
	if (fd == -1)
	{
		log_error("Error opening file", error);
		return (CMD_FAILURE);
	}
	if (dup2(fd, std_in_out) == -1)
	{
		log_error("Error duplicating file descriptor", error);
		return (CMD_FAILURE);
	}
	close(fd);
	return (CMD_SUCCESS);
}

static int process_write(t_redir *re, int type)
{
	int fd;

	if (type == MORE)
	{
		fd = open(re->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		return (handle_file(fd, STDOUT_FILENO, "open >"));
	}
	if (type == MOREMORE)
	{
		fd = open(re->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		return (handle_file(fd, STDOUT_FILENO, "open >>"));
	}
	return (CMD_FAILURE);
}

static int process_read(t_redir *re, int type)
{
	int fd;

	fd = -1;
	if (type == LESS)
	{
		fd = open(re->filename, O_RDONLY);
		return (handle_file(fd, STDIN_FILENO, "open <"));
	}
	if (type == LESSLESS)
	{
//		process_heredoc(re, &fd);
	}
	return (CMD_FAILURE);
}

int handle_directions(t_redir *redir_list)
{
	t_redir *cur;
	int		fd;
	int		status;
	
	if (redir_list == NULL)
		return (CMD_SUCCESS);
	cur = redir_list;
	while (cur)
	{
		if (cur->type == MORE || cur->type == MOREMORE)
			status = process_write(cur, cur->type);
		else if (cur->type == LESS || cur->type == LESSLESS)
		{
			fd = open(cur->filename, O_RDONLY);
			status = handle_file(fd, STDIN_FILENO, "open <");
		}
		cur = cur->next;
	}
	return (status);
}