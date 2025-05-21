/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:55:57 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/21 11:18:46 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *complete_input(void)
{
    char *input;
    char *temp;
    char **tokens;
    char *next_line;

    input = readline("\033[1;34mminishell_dude$\033[0m ");
    if(!input)
        return (NULL);
    //ft_printf("Tokenizer:\n");
    tokens = ft_tokenize(input);
    while ((long)tokens == -1)
    {
        //ft_printf("Unmatched quotes\n");
        next_line = readline("> ");
        if (!next_line)
            return (free(input), NULL);
        temp = ft_strjoin(input, "\n");
        //ft_printf("freeing input:\n");
        free(input);
        input = ft_strjoin(temp, next_line);
        //ft_printf("freeing temp:\n");
        free(temp);
        //ft_printf("freeing next line:\n");
        free(next_line);
        tokens = ft_tokenize(input);
    }
    //ft_printf("freeing split:\n");
    free_split(tokens);
    return (input);
}