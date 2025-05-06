/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:42:30 by thchau            #+#    #+#             */
/*   Updated: 2025/05/06 13:54:38 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(const char *cmd)
{
    return (!ft_memcmp(cmd, "cd", 2) ||
            !ft_memcmp(cmd, "exit", 4) ||
            !ft_memcmp(cmd, "export", 6) ||
            !ft_memcmp(cmd, "unset", 5) ||
            !ft_memcmp(cmd, "echo", 4) ||
            !ft_memcmp(cmd, "env", 3) ||
            !ft_memcmp(cmd, "pwd", 3));     
}

int execute_builtin(t_cmd *cmd, char )
