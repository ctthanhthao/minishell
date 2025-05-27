/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 15:38:33 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/25 15:57:13 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_whitespace(const char *input, int i)
{
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	return (i);
}

void token_ender(char **tokens, int tokeni)
{
    tokens[tokeni] = NULL;
}
