/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_varbls.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:37:03 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/19 14:19:37 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_variable(const char *input)
{
    char *result;
    int i;
    int start;
    char *var_name;
    char *var_value;
    
    result = ft_strdup("");
    i = 0;
    while (input[i])
    {
        if (input[i] == '$')
        {
            i++; // Skip the '$' character
            start = i;
            // Find the end of the variable name
            while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
                i++;
            var_name = ft_substr(input, start, i - start);
            // Here you would look up the variable name in your environment
            var_value = getenv(var_name); // Example using getenv
            free(var_name);
        }
        else
        {
            char temp[2];
            temp[0] = input[i];
            temp[1] = '\0';
            result = ft_strjoin(result, temp);
            i++;
        }
    }
}
