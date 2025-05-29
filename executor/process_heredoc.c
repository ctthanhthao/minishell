/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:42:49 by thchau            #+#    #+#             */
/*   Updated: 2025/05/29 09:55:15 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	expect_end_input(int fd_write, char *filename)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, filename))
		{
			free(line);
			break ;
		}
		write(fd_write, line, ft_strlen(line));
		write(fd_write, "\n", 1);
		free(line);
	}
	close(fd_write);
}

static int	redirect_last_stdin(int last_fd)
{
	if (last_fd != -1)
	{
		if (dup2(last_fd, STDIN_FILENO) == -1)
		{
			log_errno(NULL);
			close(last_fd);
			return (CMD_FAILURE);
		}
		close(last_fd);
	}
	return (CMD_SUCCESS);
}

int	process_heredoc(t_redir *redir)
{
	int		fd[2];
	int		last_fd;
	t_redir	*cur;

	last_fd = -1;
	cur = redir;
	while (cur)
	{
		if (cur->type == REDIR_HEREDOC)
		{
			if (pipe(fd) == -1)
			{
				log_errno("Error happened when pipe fd in process_heredoc");
				return (CMD_FAILURE);
			}
			expect_end_input(fd[1], cur->filename);
			if (last_fd != -1)
				close(last_fd);
			last_fd = fd[0];
		}
		cur = cur->next;
	}
	return (redirect_last_stdin(last_fd));
}
