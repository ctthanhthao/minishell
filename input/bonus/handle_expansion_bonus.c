/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expansion_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 18:44:11 by thchau            #+#    #+#             */
/*   Updated: 2025/06/08 14:35:08 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

int	handle_expansion_bonus(char ***argv_ptr, int *argc, int *capacity, t_parser *p)
{
	char	**expanded;
	int		j;
	char	**argv;

	argv = *argv_ptr;
	expanded = handle_expansion_if_any(p->tokens[p->tokeni], p->last_status,
			p->envp);
	if (expanded)
	{
		j = -1;
		while (expanded[++j])
		{
			if (expanded[j][0] != '\0')
			{
				argv = extent_argv_if_need(argv, capacity, *argc);
				if (!argv)
					return (free_split(expanded), CMD_FAILURE);
				argv[*argc] = ft_strdup(expanded[j]);
				if (!argv[*argc])
					return (free_split(expanded), CMD_FAILURE);
				(*argc)++;
			}
		}
		free_split(expanded);
	}
	return (*argv_ptr = argv, CMD_SUCCESS);
}