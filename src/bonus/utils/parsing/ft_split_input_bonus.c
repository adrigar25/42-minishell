/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_input_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:27:22 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/14 15:18:12 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

static int	is_in_quotes(const char *input, int pos)
{
	int		i;
	int		in_quote;
	char	quote_char;

	i = 0;
	in_quote = 0;
	quote_char = 0;
	while (i <= pos && input[i])
	{
		if (!in_quote && (input[i] == '\'' || input[i] == '"'))
		{
			in_quote = 1;
			quote_char = input[i];
		}
		else if (in_quote && input[i] == quote_char)
		{
			in_quote = 0;
			quote_char = 0;
		}
		i++;
	}
	return (in_quote);
}

static int	is_escaped(const char *input, int pos, int in_quote,
		char quote_char)
{
	int	count;

	if (in_quote)
		return (0);
	count = 0;
	pos--;
	while (pos >= 0 && input[pos] == '\\')
	{
		count++;
		pos--;
	}
	return (count % 2);
}

static int	is_operator_char(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '&');
}

char	**ft_split_input(const char *input, int argc)
{
	char	**args;
	int		i;
	int		arg_idx;
	int		in_quote;
	char	quote_char;
	int		start;
	char	*trimmed;

	args = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!args)
		return (NULL);
	i = 0;
	arg_idx = 0;
	in_quote = 0;
	quote_char = 0;
	while (input[i] && arg_idx < argc)
	{
		ft_skip_whitespace(input, &i);
		if (!input[i])
			break ;
		if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i]
			&& !in_quote && !is_escaped(input, i, in_quote, quote_char))
		{
			args[arg_idx++] = ft_substr((char *)input, i, 2);
			i += 2;
			ft_skip_whitespace(input, &i);
		}
		else if ((input[i] == '<' || input[i] == '>') && !in_quote
			&& !is_escaped(input, i, in_quote, quote_char))
		{
			args[arg_idx++] = ft_substr((char *)input, i, 1);
			i++;
			ft_skip_whitespace(input, &i);
		}
		else if ((input[i] == '|' || input[i] == '&') && !in_quote
			&& !is_escaped(input, i, in_quote, quote_char))
		{
			if (input[i + 1] == input[i])
			{
				args[arg_idx++] = ft_substr((char *)input, i, 2);
				i += 2;
			}
			else
			{
				args[arg_idx++] = ft_substr((char *)input, i, 1);
				i++;
			}
			while (input[i] && (input[i] == ' ' || input[i] == '\t'))
				i++;
		}
		else
		{
			start = i;
			while (input[i])
			{
				if (!in_quote && (input[i] == '\'' || input[i] == '"'))
				{
					in_quote = 1;
					quote_char = input[i];
					i++;
				}
				else if (in_quote && input[i] == quote_char)
				{
					in_quote = 0;
					quote_char = 0;
					i++;
				}
				else if (input[i] == '\\' && input[i + 1])
					i += 2;
				else if (!in_quote && is_operator_char(input[i])
					&& !is_escaped(input, i, in_quote, quote_char))
					break ;
				else if (!in_quote && ft_isspace(input[i]))
					break ;
				else
					i++;
			}
			if (i > start)
			{
				args[arg_idx++] = ft_substr((char *)input, start, i - start);
			}
		}
	}
	args[arg_idx] = NULL;
	for (int j = 0; j < arg_idx; j++)
	{
		trimmed = ft_strtrim(args[j], ' ');
		trimmed = ft_strtrim(args[j], '\t');
		free(args[j]);
		args[j] = trimmed;
	}
	return (args);
}
