/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:11:32 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/08 21:21:21 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

int main (void)
{
    char *input;
    t_cmd *cmd;
//    int i;

//    i = -1;
    //MAIN LOOP
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

        print_cmds(cmd); //Debug output
        free_cmds(cmd);

        //free the memory allocated in readline
        free(input);
    }
    return (0);
}