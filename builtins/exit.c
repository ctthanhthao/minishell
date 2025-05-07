/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:25:39 by thchau            #+#    #+#             */
/*   Updated: 2025/05/07 20:27:52 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int exit_builtin(t_cmd *cmd, char ***envp)
{
	free_cmd(cmd);
	free_envp(*envp);
	free(*envp);
	*envp = NULL;
	exit(0);
}
