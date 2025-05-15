/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:26:01 by thchau            #+#    #+#             */
/*   Updated: 2025/05/15 11:06:03 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	add_env_var(char ***envp, char *new_var)
{
	int		i;
	char	**new_envp;
	char	*copy_var;

	i = 0;
	copy_var = ft_strdup(new_var);
	if (copy_var == NULL)
		return ;
	while ((*envp)[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (new_envp == NULL)
		return (free(copy_var));
	i = 0;
	while ((*envp)[i])
	{
		new_envp[i] = (*envp)[i];
		i++;
	}
	new_envp[i] = copy_var;
	new_envp[i + 1] = NULL;
	free(*envp);
	*envp = new_envp;
}

static void	print_sorted_env(char **env)
{
	int		i;
	char	*eq;
	char	*key;
	char	**copy;

	i = 0;
	if (!env)
		return ;
	copy = selection_sort(clone_arr(env));
	while (copy[i])
	{
		eq = ft_strchr(env[i], '=');
		key = ft_substr(env[i], 0, eq - env[i]);
		ft_printf("declare -x %s=\"%s\"\n", key, (eq + 1));
		i++;
	}
	free_split(copy);
	copy = NULL;
}

int	export_builtin(t_cmd *cmd, char ***envp)
{
	int		i;
	char	*new_var;

	ft_printf("export builtin is called...\n");
	if (cmd->argv[1] == NULL)
	{
		print_sorted_env(*envp);
		return (CMD_SUCCESS);
	}
	i = 1;
	while (cmd->argv[i])
	{
		new_var = ft_strdup(cmd->argv[i]);
		if (new_var == NULL)
			return (CMD_FAILURE);
		if (ft_strchr(new_var, '=') == NULL)
			return (CMD_FAILURE);
		add_env_var(envp, new_var);
		free(new_var);
		new_var = NULL;
		i++;
	}
	return (CMD_SUCCESS);
}
