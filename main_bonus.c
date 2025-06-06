/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:11:32 by amarcz            #+#    #+#             */
/*   Updated: 2025/06/06 15:07:14 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell_bonus.h"

static char	*read_input(void)
{
	char				*input;
	static const char	*shell_prompt;

	shell_prompt = BLUE "minishell_bonus$ " RESET;
	input = readline(shell_prompt);
	if (!input)
		return (NULL);
	return (input);
}

static int	minishell_loop(char ***shell_envp, int *last_status)
{
	char	*input;
	t_ast	*root;

	while (1)
	{
		input = read_input();
		if (!input)
			break ;
		if (*input)
			add_history(input);
		root = parse_input_bonus(input, *last_status, *shell_envp);
		free(input);
		if (!root)
		{
			free_ast(root);
			continue ;
		}
		// if (!cmd->argv && cmd->redirs)
		// 	*last_status = handle_builtin_with_redirection(cmd, shell_envp,
		// 			last_status, NULL);
		// else
		// 	*last_status = execute_commands(cmd, shell_envp, last_status);
		print_ast(root); //Debug output
		free_ast(root);
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