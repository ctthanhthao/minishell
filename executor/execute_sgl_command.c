/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_sgl_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:47:05 by thchau            #+#    #+#             */
/*   Updated: 2025/05/29 12:51:53 by thchau           ###   ########.fr       */
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
			continue ;
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

static int	execute_external_cmd_without_fork(t_cmd *cmd, char ***envp,
	char *success_path)
{
	if (apply_redirections(cmd->redirs) == CMD_FAILURE)
		exit(CMD_FAILURE);
	if (execve(success_path, cmd->argv, *envp) == -1)
	{
		log_errno(NULL);
		exit(127);
	}
	return (127);
}

static int	execute_external_cmd(t_cmd *cmd, char ***envp,
	char *success_path)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (log_errno(NULL), CMD_FAILURE);
	if (pid == 0)
	{
		if (apply_redirections(cmd->redirs) == CMD_FAILURE)
			exit(CMD_FAILURE);
		if (execve(success_path, cmd->argv, *envp) == -1)
		{
			log_errno(NULL);
			exit(127);
		}
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (128 + WTERMSIG(status));
}

int	execute_single_command(t_cmd *cmd, char ***envp,
	int *last_status, bool should_fork)
{
	char	*success_path;
	int		i;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (*last_status);
	if (is_builtin(cmd->argv[0]))
		*last_status = handle_builtin_with_redirection(cmd, envp, last_status);
	else
	{
		i = 0;
		while (cmd->argv[i] && ft_strlen(cmd->argv[i]) == 0)
			i++;
		success_path = find_valid_path(cmd->argv[i], *envp);
		if (!success_path)
			return (log_errno("command not found"), 127);
		if (should_fork)
			*last_status = execute_external_cmd(cmd, envp, success_path);
		else
			*last_status = execute_external_cmd_without_fork(cmd, envp,
					success_path);
		if (success_path)
			free(success_path);
	}
	return (*last_status);
}
