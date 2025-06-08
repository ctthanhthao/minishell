/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:11:32 by amarcz            #+#    #+#             */
/*   Updated: 2025/06/07 21:50:16 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

int	minishell_loop(char ***shell_envp, int *last_status)
{
	char	*input;
	t_cmd	*cmd;

	while (1)
	{
		input = complete_input();
		if (!input)
			break ;
		if (*input)
			add_history(input);
		cmd = parse_input(input, *last_status, *shell_envp);
		free(input);
		if (!cmd)
			continue ;
		if (!cmd->argv && cmd->redirs)
			*last_status = handle_builtin_with_redirection(cmd, shell_envp,
					last_status, NULL);
		else
			*last_status = execute_commands(cmd, shell_envp, last_status);
		free_cmd(cmd);
		if (*last_status == CMD_EXIT)
			break ;
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	**shell_envp;
	int		last_status;
	int		ret;

	(void)argc;
	(void)argv;
	last_status = 0;
	shell_envp = clone_arr(envp);
	if (!shell_envp)
		return (1);
	setup_signals();
	ret = minishell_loop(&shell_envp, &last_status);
	free_split(shell_envp);
	return (ret);
}
