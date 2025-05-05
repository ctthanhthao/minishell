/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:36:53 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/05 14:50:54 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//IDENTIFY TOKEN
t_token identify_tok(char *str) //STRCMP NOT ALLOWED!!!!
{
    if (strcmp(str, "|") == 0)
        return (PIPE);
    else if (strcmp(str, ">") == 0)
        return (MORE);
    else if (strcmp(str, ">>") == 0)
        return (MOREMORE);
    else if (strcmp(str, "<") == 0)
        return (LESS);
    else if (strcmp(str, "<<") == 0)
        return (LESSLESS);
    else if (strcmp(str, "&&") == 0)
        return (AND);
    else if (strcmp(str, "||") == 0)
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
    char *token;

    head = NULL;
    last = NULL;
    token = strtok(input, " \t\n"); //split by space, tab and newline - STROK NOT ALLOWED!! - FT_SPLIT!!!

    while (token)
    {
        new = add_token(last, token);
        if (!head)
            head = new;
        last = new;
        token = strtok(NULL, " \t\n");
    }
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