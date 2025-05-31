/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:11:32 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/31 20:40:42 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

int main (int argc, char **argv, char **envp)
{
    char *input;
    t_cmd *cmd;
    char    **shell_envp;
	int		last_status = 0;
//    int status = 0;
    (void)argc;
	(void)argv;
//    int i;

//    i = -1;
    //MAIN LOOP
    shell_envp = clone_arr(envp);
	if (!shell_envp)
		return (1);
	setup_signals();
    while (1)
    {
        //DISPLAY THE PROMPT
        input = complete_input();

        //Exit the shellif user presses CTRL+D (EOF)
        if(!input)
            break;
        //add input to history
		if (*input)
        	add_history(input);

        //Parse the input
        cmd = parse_input(input, last_status, shell_envp);
		//free the memory allocated in readline
        free(input);
		if (!cmd)
		{
			free_cmd(cmd);
			continue;
		}
		if (!cmd->argv && cmd->redirs)
			last_status = handle_builtin_with_redirection(cmd, &shell_envp,
							&last_status, NULL);
		else
			last_status = execute_commands(cmd, &shell_envp, &last_status);
		//print_cmds(cmd); //Debug output
        free_cmd(cmd);
	}

    free_split(shell_envp);
    return (0);
}