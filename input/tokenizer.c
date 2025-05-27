/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarcz <amarcz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:50:42 by amarcz            #+#    #+#             */
/*   Updated: 2025/05/27 11:49:38 by amarcz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_special(char c)
{
	return (c == '>' || c == '<' || c == '&' || c == '|');
}

static int	handle_quotes(char *input, char **tokens, int *i, int *tokeni)
{
	int	start;
	int	quote;

	quote = input[*i];
	start = (*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
		return (0);
	(*i)++;
	tokens[(*tokeni)++] = ft_substr(input, start, *i - start);
	return (1);
}

static void	handle_operator(char *input, char **tokens, int *i, int *tokeni)
{
	int	start;

	start = *i;
	if (is_special(input[*i]) && input[*i] == input[*i + 1])
		*i += 2;
	else
		(*i)++;
	tokens[(*tokeni)++] = ft_substr(input, start, *i - start);
}

static void	handle_word(char *input, char **tokens, int *i, int *tokeni)
{
	int	start;

	start = *i;
	while (input[*i] && !(input[*i] == ' ' || input[*i] == '\t'
			|| is_special(input[*i])))
		(*i)++;
	tokens[(*tokeni)++] = ft_substr(input, start, *i - start);
}

char	**ft_tokenize(char *input)
{
	char	**tokens;
	int		i;
	int		tokeni;

	tokeni = 0;
	tokens = ft_calloc(1024, sizeof(char *));
	i = 0;
	if (!tokens)
		return (NULL);
	while (input[i])
	{
		i = skip_whitespace(input, i);
		if (!input[i])
			break ;
		if (input[i] == '\'' || input[i] == '\"')
		{
			if (!(handle_quotes(input, tokens, &i, &tokeni)))
				return (free_split(tokens), TOKENIZE_ERROR);
		}
		else if (is_special(input[i]))
			handle_operator(input, tokens, &i, &tokeni);
		else
			handle_word(input, tokens, &i, &tokeni);
	}
	return (token_ender(tokens, tokeni), tokens);
}
