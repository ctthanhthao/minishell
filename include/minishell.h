/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:35:36 by thchau            #+#    #+#             */
/*   Updated: 2025/05/05 17:25:12 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include "../Libft/libft.h"
#include "../Libft/ft_printf/printf.h"

// ===============================
// ENUMS
// ===============================
// TOKENS - DUDE:

typedef enum e_token
{
    CMD, //A command: ls, echo, cd
    WORD, //A generic word arguement
    PIPE, // |
    MORE, // >
    MOREMORE, // >>
    LESS, // <
    LESSLESS, // <<
    AND, // &&
    OR // ||
}   t_token;

// ===============================
// STRUCTS
// ===============================
typedef struct s_redir
{
    t_token     type;
    char            *filename;  // or delimiter for heredoc
    struct s_redir  *next;
}   t_redir;

typedef struct s_cmd
{
    char          **argv;       // arguments passed to execve or builtin
    t_redir        *redirs;     // list of redirections
    struct s_cmd   *next;       // next command in pipeline
}   t_cmd;

// ===============================
// PARSER INTERFACE - András
// ===============================
t_redir *parse_input(char *input);
void    print_tokens(t_redir *head);
char    **ft_tokenize(char *input);
void    free_tokens(t_redir *head);
void    free_split(char **arr);
char    **tokenize_line(const char *line);                            // optional internal
t_cmd   *build_cmd_list(char **tokens, char **envp);                 // optional internal
t_redir *parse_redirections(char **tokens, int *i);                  // optional internal

// ===============================
// EXECUTOR INTERFACE - Thao
// ===============================
int     execute_commands(t_cmd *cmd_list, char **envp);
int     is_builtin(const char *cmd);
int     execute_builtin(t_cmd *cmd, char ***envp);
void    apply_redirections(t_redir *redirs);

// ===============================
// CLEANUP / UTILS
// ===============================
void    free_cmd_list(t_cmd *cmd_list);
void    free_split(char **arr);

#endif
