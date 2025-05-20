/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_sgl_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:47:05 by thchau            #+#    #+#             */
/*   Updated: 2025/05/20 19:06:07 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*build_path_and_test(char **paths, char *cmd)
{
	char	*full_path;
	int		i;

	i = -1;
	while (paths[++i])
	{
		full_path = ft_strjoin_path(paths[i], cmd);
		if (full_path == NULL)
			continue;
		if (access(full_path, X_OK) == 0)
			return (full_path);
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
	full_path = build_path_and_test(paths, cmd);
	return (free_split(paths), full_path);
}

static int	execute_external_cmd(t_cmd *cmd, char ***envp)
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
		if (execve(find_valid_path(cmd->argv[0], *envp), cmd->argv, *envp) == -1)
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

int	execute_single_command(t_cmd *cmd, char ***envp, int *last_status)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (*last_status);
	if (is_builtin(cmd->argv[0]))
		*last_status = execute_builtin(cmd, envp, last_status);
	else
		*last_status = execute_external_cmd(cmd, envp);
	return (*last_status);
}
