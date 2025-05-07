/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:16:48 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/07 11:47:59 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_redirection(char *token)
{
    int i;
    
    i = -1;
    while (token[++i])
    {
        if (token[i] == '>' || token[i] == '<')
            return (1);
    }
    return (0);
}

int identify_red(char *token)
{
    if (!token)
        return (WORD);
    if (ft_strncmp(token, ">>", 3) == 0)
        return (MOREMORE);
    if (ft_strncmp(token, ">", 2) == 0)
        return (MORE);
    if (ft_strncmp(token, "<<", 3) == 0)
        return (LESSLESS);
    if (ft_strncmp(token, "<", 2) == 0)
        return (LESS);
    return (WORD);
}

int handle_redirection(t_cmd *cmd, char **tokens, int *i)
{
    t_redir *new;
    t_redir *last;

    if (!tokens[*i + 1]) //no filename after the redir
        return (ft_printf("There is a syntax error, Dude!\n"), 0);
    new = malloc(sizeof(t_redir));
    if (!new)
        return (ft_printf("Memory allocation error, Dude!\n"), 0);
    new->type = identify_red(tokens[*i]);
    new->filename = ft_strdup(tokens[*i + 1]);
    new->next = NULL;
    if (!cmd->redirs)
        cmd->redirs = new;
    else
    {
        last = cmd->redirs;
        while (last->next)
            last = last->next;
        last->next = new;
    }
    *i += 2;
    return (1);
}