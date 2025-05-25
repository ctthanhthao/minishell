/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:55:57 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/24 19:05:01 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *complete_input(void)
{
    char *input;
    char *temp;
    char **tokens;
    char *next_line;
	static const char *shell_prompt;

	shell_prompt = BLUE "minishell_dude$ " RESET;
    input = readline(shell_prompt);
    if(!input)
        return (NULL);
    //ft_printf("Tokenizer:\n");
    tokens = ft_tokenize(input);
    while ((long)tokens == -1)
    {
		tokens = NULL;
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