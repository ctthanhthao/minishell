/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:27:32 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/29 12:12:20 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_quoted(char *token)
{
    int len;

    len = ft_strlen(token);
    if ((token[0] == '\'' && token[len - 1] == '\'')
        ||  (token[0] == '\"' && token[len - 1] == '\"')) 
        return (1);
    return (0);
}