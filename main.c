/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:11:32 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/23 09:14:51 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

static bool	save_original_std_inout(int *stdin_bk, int *stdout_bk)
{
	*stdout_bk = dup(STDOUT_FILENO);
	if (*stdout_bk == -1)
	{
		log_error("Error saving STDOUT_FILENO", "dup");
		return (false);
	}
	*stdin_bk = dup(STDIN_FILENO);
	if (*stdin_bk == -1)
	{
		log_error("Error saving STDIN_FILENO", "dup");
		return (false);
	}
	return (true);
}

static void	restore_original_std_inout(int stdin_bk, int stdout_bk)
{
	if (stdout_bk != -1)
	{
		dup2(stdout_bk, STDOUT_FILENO);
		close(stdout_bk);
	}
	if (stdin_bk != -1)
	{
		dup2(stdin_bk, STDIN_FILENO);
		close(stdin_bk);
	}
}

int main (int argc, char **argv, char **envp)
{
    char *input;
    t_cmd *cmd;
    char    **shell_envp;
	int		stdout_bk;
	int		stdin_bk;
	int		last_exit_status = 0;
	bool	redirected;
//    int status = 0;
    (void)argc;
	(void)argv;
//    int i;

//    i = -1;
    //MAIN LOOP
	stdout_bk = -1;
	stdin_bk = -1;
	redirected = false;
    shell_envp = clone_arr(envp);
	if (!shell_envp)
		return (1);
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
        cmd = parse_input(input, last_exit_status, envp);
		//free the memory allocated in readline
        free(input);
		if (!cmd || !cmd->argv || !cmd->argv[0])
		{
			free_cmd(cmd);
			continue;
		}
		if (cmd->redirs)
			redirected = save_original_std_inout(&stdin_bk, &stdout_bk);
			
		if (apply_redirections(cmd->redirs) == CMD_FAILURE)
		{
			last_exit_status = 1;
			if (redirected)
				restore_original_std_inout(stdin_bk, stdout_bk);
			free_cmd(cmd);
			continue;
		}
		execute_commands(cmd, &shell_envp, &last_exit_status);
        // Restore the original STDIN_FILENO STDOUT_FILENO
		if (redirected)
		{
			restore_original_std_inout(stdin_bk, stdout_bk);
			redirected = false;	
		}

		//print_cmds(cmd); //Debug output
        free_cmd(cmd);
	}

    free_split(shell_envp);
    return (0);
}