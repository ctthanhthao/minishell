/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:09:11 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/15 14:06:27 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_operator(char *token)
{
    return (ft_strncmp(token, "||", 2) == 0 ||
            ft_strncmp(token, "&&", 2) == 0 ||
            ft_strncmp(token, "|", 1) == 0 );
}

int ft_is_redirection(char *token)
{
    return (ft_strncmp(token, ">>", 2) == 0 ||
            ft_strncmp(token, ">", 1) == 0 ||
            ft_strncmp(token, "<<", 2) == 0 ||
            ft_strncmp(token, "<", 1) == 0);
}

int validate_tokens(char **tokens)
{
    int i;

    i = 0;
    if (!tokens || !tokens[0])
        return (1); // No tokens to validate - empty input
    if (is_operator(tokens[0]))
            return (ft_printf("Whoa, Dude!\n"),
                ft_printf("You can't start a command with '&&' or '||'. Seriously, Bro!\n"), 0); // First token cannot be an operator or redirection
    while (tokens[i])
    {
        if (is_operator(tokens[i]))
        {
            //No consecutive operators
            if (!tokens[i + 1] || is_operator(tokens[i + 1]))
                return (ft_printf("Yo, Bro!\n"),
                    ft_printf("You've totally messed up the syntax at %s!\n", tokens[i]), 0);
        }
        else if (is_redirection(tokens[i]))
        {
            //Redirection must be followed by a filename
            if (!tokens[i + 1] || is_operator(tokens[i + 1]) || ft_is_redirection(tokens[i + 1]))
                return (ft_printf("Not cool, Dude!\n"),
                    ft_printf("Syntax error near %s redirection, Bro!\n", tokens[i]), 0);
        }
        i++;
    }
    return (1); // All tokens are valid
}
