/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_sgl_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:47:05 by thchau            #+#    #+#             */
/*   Updated: 2025/05/19 22:30:20 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	test_in_dev_null(char *full_path, char *cmd, char **envp)
{
	int	fd;

	fd = open("/dev/null", O_WRONLY);
	if (fd != -1)
	{
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
		close(fd);
	}
	execve(full_path, (char *[]){cmd, NULL}, envp);
	exit(126);
}

static char	*build_and_test(char **paths, char *cmd, char **envp)
{
	char	*full_path;
	pid_t	pid;
	int		status;
	int		i;

	i = -1;
	while (paths[++i])
	{
		full_path = ft_strjoin_path(paths[i], cmd);
		pid = fork();
		if (pid == 0)
			test_in_dev_null(full_path, cmd, envp);
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status) && WEXITSTATUS(status) != 126)
				return (full_path);
		}
		if (full_path)
			free(full_path);
	}
	return (NULL);
}

/**
 * If cmd starts with a slash, treat as path
 * Else we'll build full path which is combined from valid path in env 
 * with given cmd
 */
static char	*find_valid_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	int		i;

	path_env = NULL;
	full_path = NULL;
	if (ft_strchr(cmd, '/'))
		return (strdup(cmd));
	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break ;
		}
	}
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full_path = build_and_test(paths, cmd, envp);
	return (free_split(paths), full_path);
}

static int	execute_external_cmd(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (log_error("Error happened in fork during"
				"execute_single_command",
				"fork"), CMD_FAILURE);
	if (pid == 0)
	{
		if (execve(find_valid_path(cmd->argv[0], envp), cmd->argv, envp) == -1)
		{
			log_error("Error happened in execve during execute_single_command",
				cmd->argv[0]);
			exit(127);
		}
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (128 + WTERMSIG(status));
}

int	execute_single_command(t_cmd *cmd, char **envp, int *last_status)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (CMD_SUCCESS);
	if (is_builtin(cmd->argv[0]))
		*last_status = execute_builtin(cmd, &envp, last_status);
	else
		*last_status = execute_external_cmd(cmd, envp);
	return (CMD_SUCCESS);
}
