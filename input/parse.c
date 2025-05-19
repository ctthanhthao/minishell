/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:36:53 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/19 15:32:04 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//IDENTIFY TOKEN
t_token identify_tok(char *str)
{
    if (ft_strncmp(str, "||", 2) == 0)
        return (PIPE);
    else if (ft_strncmp(str, ">>", 2) == 0)
        return (MORE);
    else if (ft_strncmp(str, ">", 1) == 0)
        return (MOREMORE);
    else if (ft_strncmp(str, "<<", 2) == 0)
        return (LESS);
    else if (ft_strncmp(str, "<", 1) == 0)
        return (LESSLESS);
    else if (ft_strncmp(str, "&&", 2) == 0)
        return (AND);
    else if (ft_strncmp(str, "|", 1) == 0)
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
t_cmd *parse_input(char *input)
{
    char **tokens;
    int i;
    int argv_i;
    t_cmd *head;
    t_cmd *curr;
    t_cmd *prev;
    // t_redir *new;
    // t_token id;
    // int first_word;
    
    tokens = ft_tokenize(input);
    if (!validate_tokens(tokens) || !tokens)
    {
        free_split(tokens);
        return (NULL);
    }
    i = 0;
    argv_i = 0;
    head = NULL;
    curr = NULL;
    prev = NULL;
    // first_word = 1;

    //ft_printf("Loop: \n");
    while (tokens[i])
    {
        //ft_printf("First if:\n");
        if (!curr)
        {
            curr = malloc(sizeof(t_cmd));
            if (!curr)
                return (NULL);
            curr->argv = malloc(sizeof(char *) * 1024);
            curr->redirs = NULL;
            curr->next = NULL;
            curr->next_type = CMD_NONE;
            argv_i = 0;
            if (!head)
                head = curr;
            else
                prev->next = curr;       
        }
        //ft_printf("Second if:\n");
        if (ft_strncmp(tokens[i], "||", 2) == 0)
        {
            //ft_printf("OR_IF detected\n");
            if (curr)
                curr->next_type = CMD_OR_IF;
            curr->argv[argv_i] = NULL;
            i++; // Skip || symbol
            prev = curr;
            curr = NULL;
            continue;
        }
        else if (ft_strncmp(tokens[i], "|", 1) == 0)
        {
            //ft_printf("Pipe detected\n");
            if (curr)
            {
                curr->next_type = CMD_PIPE;
                //ft_printf("prev->next_type (%d) = CMD_PIPE\n", curr->next_type);
            }
                
            curr->argv[argv_i] = NULL;
            i++; // Skip pipe symbol
            prev = curr;
            curr = NULL;
            continue;
        }
        else if (ft_strncmp(tokens[i], "&&", 2) == 0)
        {
            //ft_printf("AND_IF detected\n");
            if (curr)
            {
                curr->next_type = CMD_AND_IF;
                //ft_printf("prev->next_type (%d) = CMD_AND_IF\n", curr->next_type);
            }
            curr->argv[argv_i] = NULL;
            i++; // Skip && symbol
            prev = curr;
            curr = NULL;
            continue;
        }
        //ft_printf("Third if:\n");
        if (is_redirection(tokens[i]))
        {
            if (!handle_redirection(curr, tokens, &i))
            {
                free_split(tokens);
                free_cmds(head);
                return (NULL);
            }
            continue;
        }
        //ft_printf("Rest:\n");
        //expand_dollar(NULL, 0, env);
        curr->argv[argv_i++] = ft_strdup(tokens[i++]);// 
        // new = malloc(sizeof(t_redir));
        // if (!new)
        //     return (free_split(tokens), NULL);
        // new ->filename = ft_strdup(tokens[i]);
        // id = identify_tok(tokens[i]);
        // if (id == WORD && first_word)
        //     new->type = CMD;
        // else
        //     new->type = id;
        // first_word = (new->type == PIPE);
        // new->next = NULL;
        // if (!head)
        //     head = new;
        // else
        //     last->next = new;
        // last = new;
        // i++;
    }
    //ft_printf("End loop\n");
    if (curr)
        curr->argv[argv_i] = NULL;
    prev = curr;
    free_split(tokens);
    return (head);
}

//DEBUG FUNCTION TO PRINT TOKENS:

void print_cmds(t_cmd *cmd)
{
    const char  *token_names[] = {
        "CMD", "WORD", "PIPE", "MORE", "MOREMORE", "LESS", "LESSLESS", \
        "AND", "OR"
    };
    const char *cmd_type_strs[] = {"NONE", "AND_IF", "OR_IF", "PIPE"};
    int i;
    t_redir *r;

    i = -1;
    while (cmd)
    {
        ft_printf("==== Command === \n");

        if (cmd->argv)
        {
            while (cmd->argv[++i])
            {
                ft_printf("Arg[%d]: %s\n", i, cmd->argv[i]);
            }
            ft_printf("Next type: %s\n", cmd_type_strs[cmd->next_type]);
        }
        r = cmd ->redirs;
        while (r)
        {
            ft_printf("Redir: %-10s | Type: %s\n", r->filename, token_names[r->type]);
            r = r->next;
        }
        cmd = cmd->next;
        i = -1;
    }
}

//Free command list
void free_cmds(t_cmd *cmd)
{
    t_cmd *tmp_cmd;
    int i;
    t_redir *r;
    t_redir *tmp_redir;

    while (cmd)
    {
        tmp_cmd = cmd;
        cmd = cmd->next;

        if (tmp_cmd->argv)
        {
            i = 0;
            while (tmp_cmd->argv[i])
                free (tmp_cmd->argv[i++]);
            free (tmp_cmd->argv);
        }
        r = tmp_cmd->redirs;
        while (r)
        {
            tmp_redir = r;
            r = r->next;
            free(tmp_redir->filename);
            free(tmp_redir);
        }
        free(tmp_cmd);
    }
}