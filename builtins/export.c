/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:26:01 by thchau            #+#    #+#             */
/*   Updated: 2025/05/28 19:11:04 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*extract_key(const char *entry)
{
	char	*eq;

	eq = ft_strchr(entry, '=');
	if (eq)
		return (ft_substr(entry, 0, eq - entry));
	else
		return (ft_strdup(entry));
}

static char	**add_env_var(char **envp, const char *new_var)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (!new_envp)
		return (NULL);
	i = -1;
	while (envp[++i])
		new_envp[i] = ft_strdup(envp[i]);
	new_envp[i] = ft_strdup(new_var);
	new_envp[i + 1] = NULL;
	free_split(envp);
	return (new_envp);
}

static bool	update_env(char **entry_env, const char *new_var, size_t key_len)
{
	if (!new_var[key_len])
		return (true);
	else if (new_var[key_len] == '=')
	{
		free(*entry_env);
		*entry_env = ft_strdup(new_var);
		return (true);
	}
	else
		return (false);
}

static char	**update_or_add_env(char **envp, const char *new_var)
{
	char	*key;
	int		i;
	size_t	key_len;

	key = extract_key(new_var);
	if (!key)
		return (NULL);
	key_len = ft_strlen(key);
	i = -1;
	while (envp[++i])
	{
		if ((ft_strncmp(envp[i], key, key_len) == 0) && (!envp[i][key_len]
			|| envp[i][key_len] == '='))
		{
			if (update_env(&envp[i], new_var, key_len))
			{
				free(key);
				return (envp);
			}
			break ;
		}
	}
	free(key);
	return (add_env_var(envp, new_var));
}

int	export_builtin(t_cmd *cmd, char ***envp)
{
	int	i;
	int	status;

	status = CMD_SUCCESS;
	if (!envp || !*envp)
		return (log_error("env is null", "export_builtin"), CMD_FAILURE);
	if (!cmd->argv[1])
		return (print_sorted_env(*envp), CMD_SUCCESS);
	i = 1;
	while (cmd->argv[i])
	{
		if (!is_valid_identifier(cmd->argv[i]))
		{
			log_error("export: invalid identifier", "not a valid identifier");
			i++;
			status = CMD_FAILURE;
			continue ;
		}
		*envp = update_or_add_env(*envp, cmd->argv[i]);
		if (!*envp)
			return (log_error("memory allocation failed", NULL), CMD_FAILURE);
		i++;
	}
	return (status);
}
