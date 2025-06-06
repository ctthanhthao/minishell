/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 19:28:36 by thchau            #+#    #+#             */
/*   Updated: 2025/06/06 12:29:16 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell_bonus.h"

static int	is_special(char c)
{
	return (c == '>' || c == '<' || c == '&' || c == '|'
		|| c == '(' || c == ')');
}

static void	handle_quotes(char *input, char **tokens, int *i, int *tokeni)
{
	int	start;
	int	quote;

	quote = input[*i];
	start = (*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	(*i)++;
	tokens[(*tokeni)++] = ft_substr(input, start, *i - start);
}

static void	handle_operator(char *input, char **tokens, int *i, int *tokeni)
{
	int	start;

	start = *i;
	if (is_special(input[*i]))
	{
		if (input[*i] == input[*i + 1] && (input[*i] == '&' || input[*i] == '|'
			|| input[*i] == '<' || input[*i] == '>'))
			*i += 2;
		else 
			(*i)++;
		tokens[(*tokeni)++] = ft_substr(input, start, *i - start);
	}
}

static void	handle_word(char *input, char **tokens, int *i, int *tokeni)
{
	int		capacity;
	char	*buffer;
	int		buf_i;
	char	quote;

	buf_i = 0;
	capacity = INITIAL_CAPACITY;
	buffer = ft_calloc(capacity, sizeof(char));
	while (input[*i] && !(input[*i] == ' ' || input[*i] == '\t'
			|| is_special(input[*i])))
		{
			if (buf_i > capacity - 1)
			{
				capacity *= 2;
				tokens = realloc(tokens, capacity * sizeof(char *));
				if (!tokens)
					return ;
			}
			if (input[*i] == '\'' || input[*i] == '\"')
			{
				quote = input[(*i)++];
				while (input[*i] && input[*i] != quote)
					buffer[buf_i++] = input[(*i)++];
				if (input[*i] == quote)
					(*i)++;
			}
			else
				buffer[buf_i++] = input[(*i)++];
		}
		buffer[buf_i] = '\0';
		tokens[(*tokeni)++] = buffer;
}

char	**ft_tokenize_bonus(char *input)
{
	char	**tokens;
	int		i;
	int		tokeni;
	int		capacity;

	tokeni = 0;
	capacity = INITIAL_CAPACITY;
	tokens = ft_calloc(capacity, sizeof(char *));
	i = 0;
	if (!tokens)
		return (NULL);
	while (input[i])
	{
		i = skip_whitespace(input, i);
		if (!input[i])
			break ;
		if (tokeni > capacity - 1)
		{
			capacity *= 2;
			tokens = realloc(tokens, capacity * sizeof(char *));
			if (!tokens)
				return (NULL);
		}
		if (input[i] == '\'' || input[i] == '\"')
			handle_quotes(input, tokens, &i, &tokeni);
		else if (is_special(input[i]))
			handle_operator(input, tokens, &i, &tokeni);
		else
			handle_word(input, tokens, &i, &tokeni);
	}
	return (token_ender(tokens, tokeni), tokens);
}
