/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:32:07 by thchau            #+#    #+#             */
/*   Updated: 2025/05/07 19:42:19 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	check_env_exists(char **envp, char *var_name)
{
	int		i;
	int		var_len;

	i = 0;
	var_len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, var_len) == 0 &&
						envp[i][var_len] == '=')
			return (1);
		i++;
	}
	return (0);
}

static void	unset_env_var(char ***envp, char *var_name)
{
	int		len_env;
	char	**new_envp;
	int		i;
	int		j;

	len_env = 0;
	i = 0;
	j = 0;
	if (!check_env_exists(*envp, var_name))
		return ;
	while ((*envp)[len_env])
		len_env++;
	new_envp = malloc(sizeof(char *) * len_env);
	if (new_envp == NULL)
		return ;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], var_name, ft_strlen(var_name)) == 0 &&
				(*envp)[i][ft_strlen(var_name)] == '=')
			free((*envp)[i]);
		else
			new_envp[j++] = (*envp)[i];	
		i++;
	}
	new_envp[j] = NULL, free(*envp), *envp = new_envp;
}

int	unset_builtin(t_cmd *cmd, char ***envp)
{
	int		i;
	
	if (cmd->argv[1] == NULL)
		return (0);
	i = 1;
	while (cmd->argv[i])
	{
		unset_env_var(envp, cmd->argv[i]);
		i++;
	}
	return (0);
}
