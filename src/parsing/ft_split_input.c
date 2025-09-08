/* ************************************************************************** */
/*                          			if (i > start)
				args[arg_idx++] = ft_substr((char *)input, start, i
						- start);                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>             +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:58:32 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/05 17:05:09 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	return (c == '<' || c == '>' || c == '|');
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
			&& !is_escaped(input, i, in_quote, quote_char))
		{
			// Handle << or >> as a single argument
			args[arg_idx++] = ft_substr((char *)input, i, 2);
			i += 2;
			ft_skip_whitespace(input, &i);
		}
		else if ((input[i] == '<' || input[i] == '>') && !is_escaped(input, i,
				in_quote, quote_char))
		{
			args[arg_idx++] = ft_substr((char *)input, i, 1);
			i++;
			ft_skip_whitespace(input, &i);
		}
		else if (input[i] == '|' && !is_escaped(input, i, in_quote, quote_char))
		{
			// Handle pipe as a single argument
			args[arg_idx++] = ft_substr((char *)input, i, 1);
			i++;
			// Skip whitespace manually después del pipe
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
		trimmed = ft_trim(args[j], ' ');
		trimmed = ft_trim(args[j], '\t');
		free(args[j]);
		args[j] = trimmed;
	}
	return (args);
}
