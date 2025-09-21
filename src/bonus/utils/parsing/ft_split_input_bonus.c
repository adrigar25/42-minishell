/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_input_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:27:22 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 00:44:29 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

int	is_in_quotes(const char *input, int pos)
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
	return (c == '<' || c == '>' || c == '|');
}

char	**ft_split_input(const char *input, int argc)
{
	char	**args;
	int		i;
	int		arg_idx;
	int		start;

	args = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!args)
		return (NULL);
	i = 0;
	arg_idx = 0;
	while (input[i] && arg_idx < argc)
	{
		ft_skip_whitespace(input, &i);
		if (is_operator_char(input[i]) && !is_escaped(input, i, 0, 0))
		{
			args[arg_idx++] = ft_substr((char *)input, i, 1 + (input[i
						+ 1] == input[i]));
			i += 1 + (input[i + 1] == input[i]);
			ft_skip_whitespace(input, &i);
			continue ;
		}
		start = i;
		while (input[i] && !((!is_in_quotes(input, i)
					&& (is_operator_char(input[i]) || ft_isspace(input[i]))
					&& !is_escaped(input, i, 0, 0))))
			i += 1 + (input[i] == '\\' && input[i + 1]);
		if (i > start)
			args[arg_idx++] = ft_substr((char *)input, start, i - start);
	}
	args[arg_idx] = NULL;
	return (args);
}
