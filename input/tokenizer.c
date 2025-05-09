/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:50:42 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/07 11:51:34 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **ft_tokenize(char *input)
{
    char **tokens;
    int i;
    int start;
    int tokeni;
    char quote;

    i = 0;
    tokeni = 0;
    tokens = malloc(sizeof(char *) * 1024);
    if (!tokens)
        return (NULL);
    while (input[i])
    {
        //Skip whitespace
        if (input[i] == ' ' || input[i] == '\t')
        {
            i++;
            continue;
        }

        //Handle quotes
        if (input[i] == '\'' || input[i] == '\"')
        {
            quote = input[i++];
            start = i;
            while (input[i] && input[i] != quote)
                i++;
            tokens[tokeni++] = ft_substr(input, start, i - start);
            if (input[i] == quote)
                i++;
            continue;
        }

        //Double tokens
        if (input[i] == '>' || input[i] == '<' || input[i] == '&' || input[i] == '|')
        {
            start = i;
            if ((input[i] == '>' || input[i] == '<' || input[i] == '&' || input[i] == '|') && input[i] == input[i + 1])
                i += 2;
            else
                i += 1;
            tokens[tokeni++] = ft_substr(input, start, i - start);
            continue;
        }

        //Regular words
        start = i;
        while (input[i] && !(input[i] == ' ' || input[i] == '\t' || (input[i] == '>' || input[i] == '<' || input[i] == '&' || input[i] == '|')))
            i++;
        tokens[tokeni++] = ft_substr(input, start, i - start);
    }
    tokens[tokeni] = NULL;
    return (tokens);
}