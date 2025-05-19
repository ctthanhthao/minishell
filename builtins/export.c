/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:26:01 by thchau            #+#    #+#             */
/*   Updated: 2025/05/19 09:26:55 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	is_valid_identifier(const char *s)
{
	if (!s || (!ft_isalpha(*s) && *s != '_'))
		return (0);
	while (*s && *s != '=')
	{
		if (!ft_isalnum(*s) && *s != '_')
			return (0);
		s++;
	}
	return (1);
}

static void	print_sorted_env(char **env)
{
	int		i;
	char	*eq;
	char	*key;
	char	**copy;

	i = 0;
	if (!env || !*env)
		return ;
	ft_printf("clone_arr started...\n");
	copy = clone_arr(env);
	if (copy == NULL)
		return ;
	ft_printf("clone_arr started...\n");
	copy = selection_sort(copy);
	if (copy == NULL)
	{
		free_split(copy);
		return ;
	}
	while (copy[i])
	{
		eq = ft_strchr(copy[i], '=');
		if (eq != NULL)
		{
			key = ft_substr(copy[i], 0, eq - copy[i]);
			ft_printf("declare -x %s=\"%s\"\n", key, (eq + 1));
			free(key);
		}
		else
			ft_printf("declare -x %s\n", copy[i]);	
		i++;
	}
	free_split(copy);
	copy = NULL;
}

static char **add_env_var(char **envp, char *new_var)
{
	char	**new_envp;
	int		i;
	
	i = 0;
	while (envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (new_envp == NULL)
		return (NULL);
	i = 0;
	ft_printf("add_env_var started....\n");
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		i++;
	}
	new_envp[i] = ft_strdup(new_var);
	new_envp[i + 1] = NULL;
	return (new_envp);
}

static char	**update_or_add_env(char **envp, char *new_var)
{
	int		i;
	char	*key;
	char	*eq;
	char	**result;

	eq = ft_strchr(new_var, '=');
	if (eq)
		key = ft_substr(new_var, 0, eq - new_var);
	else
		key = ft_strdup(new_var);
	if (!key)
		return (NULL);
	printf("key is %s\n", key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0
				&& (envp[i][ft_strlen(key)] == '='
				|| envp[i][ft_strlen(key)] == '\0'))
		{
			printf("equal key.....envp[%i] is %s\n", i, envp[i]);
			if (eq)
			{
				free(envp[i]);
				envp[i] = ft_strdup(new_var);
			}
			return (free(key), envp);
		}
		i++;
	}
	result = add_env_var(envp, new_var);
	free(key);
	return (result);
}

int	export_builtin(t_cmd *cmd, char ***envp)
{
	int		i;
	char	**new_env;

	ft_printf("export builtin is called...\n");
	if (envp == NULL || *envp == NULL)
		return (log_error("Environment is NUL", "export_builtin"), CMD_FAILURE);
	if (cmd->argv[1] == NULL)
		return (print_sorted_env(*envp), CMD_SUCCESS);
	i = 1;
	while (cmd->argv[i])
	{
		if (!is_valid_identifier(cmd->argv[i]))
		{
			log_error("export an invalid identifier\n", "is_valid_identifier");
			continue;
		}
		new_env = update_or_add_env(*envp, cmd->argv[i]);
		if (new_env == NULL)
			return (CMD_FAILURE);
		if (new_env != *envp)
			free_split(*envp);
		*envp = new_env;
		i++;
	}
	return (CMD_SUCCESS);
}
