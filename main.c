/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:11:32 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/19 20:57:43 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

int main (int argc, char **argv, char **envp)
{
    char *input;
    t_cmd *cmd;
    char    **shell_envp;
	int		stdout_bk;
	int		last_exit_status = 0;
//    int status = 0;
    (void)argc;
	(void)argv;
//    int i;

//    i = -1;
    //MAIN LOOP
    shell_envp = clone_arr(envp);
	if (!shell_envp)
		return (1);
    while (1)
    {
        //DISPLAY THE PROMPT
        input = readline("\033[1;34mminishell_dude$\033[0m ");

        //Exit the shellif user presses CTRL+D (EOF)
        if(!input)
            break;
        
        //add input to history
		if (*input)
        	add_history(input);

        //Parse the input
        cmd = parse_input(input);
		//free the memory allocated in readline
        free(input);
		if (!cmd || !cmd->argv || !cmd->argv[0])
		{
			free_cmd(cmd);
			continue;
		}
        // Save the original STDOUT_FILENO
		stdout_bk = dup(STDOUT_FILENO);
		if (stdout_bk == -1)
		{
			log_error("Error saving STDOUT_FILENO", "dup");
			return (CMD_FAILURE);
		}
		if (apply_redirections(cmd->redirs) == CMD_FAILURE)
		{
			last_exit_status = 1;
			free_cmd(cmd);
			continue;
		}
		execute_commands(cmd, shell_envp, &last_exit_status);
        // Restore the original STDOUT_FILENO
		if (dup2(stdout_bk, STDOUT_FILENO) == -1)
		{
			log_error("Error restoring STDOUT_FILENO", "dup2");
			free_cmd(cmd);
			continue;
		}
		close(stdout_bk);
//        print_cmds(cmd); //Debug output
        free_cmds(cmd);
    }
    free_split(shell_envp);
    return (0);
}