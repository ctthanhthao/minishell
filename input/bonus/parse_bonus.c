/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 20:39:39 by thchau            #+#    #+#             */
/*   Updated: 2025/06/08 07:58:03 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

static t_parser init_parser_data(char **tokens, int last_status, char **envp)
{
	t_parser	parser;
	int			i;

	i = 0;
	while (tokens[i])
		i++;
	parser.tokeni = 0;
	parser.token_count = i;
	parser.tokens = tokens;
	parser.last_status = last_status;
	parser.envp = envp;
	return (parser);
}

t_ast	*parse_input_bonus(char *input, int last_status, char **envp)
{
	char		**tokens;
	t_ast		*head;
	t_parser	parser_data;

	if (!check_unclosed_quotes(input) || !check_invalid_chars(input)
		|| !check_unclosed_parenthesis(input))
		return (NULL);
	tokens = ft_tokenize_bonus(input);
	if (!validate_tokens(tokens) || !tokens || !wildcard_check(tokens))
		return (free_split(tokens), NULL);
	// for (int i = 0; tokens[i]; i++)
	// {
	// 	ft_printf("token[%i] : %s\n", i, tokens[i]);
	// }
	parser_data = init_parser_data(tokens, last_status, envp);
	head = parse_expression(&parser_data);
	if (!head)
		return (free_split(tokens), NULL);
	free_split(tokens);
	return (head);
}