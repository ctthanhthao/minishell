/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:11:32 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/19 15:32:20 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

int main (int argc, char **argv, char **envp)
{
    char *input;
    t_cmd *cmd;
    char    **shell_envp;
//    int status = 0;
    (void)argc;
	(void)argv;
//    int i;

//    i = -1;
    //MAIN LOOP
    shell_envp = clone_arr(envp);
    while (1)
    {
        //DISPLAY THE PROMPT
        input = readline("\033[1;34mminishell_dude$\033[0m ");

        //Exit the shellif user presses CTRL+D (EOF)
        if(!input)
            break;
        
        //add input to history
        add_history(input);

        //Parse the input
        cmd = parse_input(input);
        // if (apply_redirections(cmd->redirs) == CMD_FAILURE)
		// {
		// 	status = 1;
		// 	free_cmd(cmd);
		// 	continue;
		// }
       // execute_commands(cmd, shell_envp, &status);
        
        print_cmds(cmd); //Debug output
        free_cmds(cmd);

        //free the memory allocated in readline
        free(input);
    }
    free_split(shell_envp);
    return (0);
}