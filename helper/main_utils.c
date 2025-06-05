/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:55:57 by amarcz            #+#    #+#             */
/*   Updated: 2025/06/05 15:25:38 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*complete_input(void)
{
	char				*input;
	char				*temp;
	char				**tokens;
	char				*next_line;
	static const char	*shell_prompt;

	shell_prompt = BLUE "minishell_dude$ " RESET;
	input = readline(shell_prompt);
	if (!input)
		return (NULL);
	tokens = NULL;
	while ((long)tokens == -1)
	{
		tokens = NULL;
		next_line = readline("> ");
		if (!next_line)
			return (free(input), NULL);
		temp = ft_strjoin(input, "\n");
		free(input);
		input = ft_strjoin(temp, next_line);
		free(temp);
		free(next_line);
		tokens = ft_tokenize(input);
	}
	return (free_split(tokens), input);
}
