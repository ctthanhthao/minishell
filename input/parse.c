/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:36:53 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/05 17:25:41 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//IDENTIFY TOKEN
t_token identify_tok(char *str)
{
    if (ft_strncmp(str, "|", 2) == 0)
        return (PIPE);
    else if (ft_strncmp(str, ">", 2) == 0)
        return (MORE);
    else if (ft_strncmp(str, ">>", 3) == 0)
        return (MOREMORE);
    else if (ft_strncmp(str, "<", 2) == 0)
        return (LESS);
    else if (ft_strncmp(str, "<<", 3) == 0)
        return (LESSLESS);
    else if (ft_strncmp(str, "&&", 3) == 0)
        return (AND);
    else if (ft_strncmp(str, "||", 3) == 0)
        return (OR);
    return (WORD); //DEFAULT
}

//Add a new token node to the list
t_redir *add_token(t_redir *last, char *value)
{
    t_redir *new;

    new = malloc(sizeof(t_redir));
    if (!new)
        return (NULL);
    new->filename = ft_strdup(value);
    new->type = identify_tok(value);
    new->next = NULL;
    if (last)
        last->next = new;
    return (new);
}

//Main parsing function:
t_redir *parse_input(char *input)
{
    t_redir *head;
    t_redir *last;
    t_redir *new;
    t_token id;
    char **tokens;
    int first_word;
    int i;

    i = 0;
    first_word = 1;
    head = NULL;
    last = NULL;
    tokens = ft_tokenize(input);
    if (!tokens)
        return (NULL);

    while (tokens[i])
    {
        new = malloc(sizeof(t_redir));
        if (!new)
            return (free_split(tokens), NULL);
        new ->filename = ft_strdup(tokens[i]);
        id = identify_tok(tokens[i]);
        if (id == WORD && first_word)
            new->type = CMD;
        else
            new->type = id;
        first_word = (new->type == PIPE);
        new->next = NULL;
        if (!head)
            head = new;
        else
            last->next = new;
        last = new;
        i++;
    }
    free_split(tokens);
    return (head);
}

//DEBUG FUNCTION TO PRINT TOKENS:

void print_tokens(t_redir *head)
{
    const char  *token_names[] = {
        "CMD", "WORD", "PIPE", "MORE", "MOREMORE", "LESS", "LESSLESS", \
        "AND", "OR"
    };
    while (head)
    {
        ft_printf("Token: %-10s | Type: %s\n", head->filename, token_names[head->type]);
        head = head->next;
    }
}

//Free token list
void free_tokens(t_redir *head)
{
    t_redir *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->filename);
        free(tmp);
    }
}