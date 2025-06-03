/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:08:16 by thchau            #+#    #+#             */
/*   Updated: 2025/06/03 11:49:03 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*remove_quotes_if_need(char *arg)
{
	bool	in_single;
	bool	in_double;
	int		i;
	char	*re;

	in_single = false;
	in_double = false;
	i = 0;
	re = ft_calloc((ft_strlen(arg) + 1), sizeof(char));
	if (!re)
		return (NULL);
	ft_printf("Remove quotes arg: \n");
	ft_printf("%s \n", arg);
	while (*arg)
	{
		if (*arg == '\'' && !in_double)
			in_single = !in_single;
		else if (*arg == '\"' && !in_single)
			in_double = !in_double;
		else
			re[i++] = *arg;
		arg++;
	}
	re[i] = '\0';
	ft_printf("Remove quotes arg after: \n");
	ft_printf("%s \n", arg);
	return (re);
}

int	echo_builtin(t_cmd *cmd, int *status)
{
	int		i;
	int		newline;

	i = 1;
	newline = 1;
	if (cmd->argv[i] && !ft_strcmp(cmd->argv[i], "-n"))
	{
		newline = 0;
		i++;
	}
	while (cmd->argv[i])
	{
		ft_printf("%s", cmd->argv[i]);
		if (cmd->argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (*status = 0, CMD_SUCCESS);
}
