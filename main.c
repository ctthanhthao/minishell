/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:11:32 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/05 14:15:02 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

int main (void)
{
    char *input;
    t_redir *tokens;
    int i;

    i = -1;
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
        tokens = parse_input(input);

        print_tokens(tokens); //Debug output
        free_tokens(tokens);

        //free the memory allocated in readline
        free(input);
    }
    return (0);
}