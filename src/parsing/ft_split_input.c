/* ************************************************************************** */
/*                                                                            */
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

char	**ft_split_input(const char *input, int argc)
{
	char	**args;
	int		i;
	int		arg_idx;
	int		in_quote;
	char	quote_char;

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
		if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i])
		{
			// Handle << or >> as a single argument
			args[arg_idx++] = ft_substr((char *)input, i, 2);
			i += 2;
			ft_skip_whitespace(input, &i);
			int start = i;
			while (input[i] && (!ft_isspace(input[i]) || in_quote))
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
				else
					i++;
			}
			if (i > start)
				args[arg_idx++] = ft_substr((char *)input, start, i - start);
		}
		else if (input[i] == '<' || input[i] == '>')
		{
			// Handle <infile.txt or >outfile.txt as a single argument
			args[arg_idx++] = ft_substr((char *)input, i, 1);
			i++;
			ft_skip_whitespace(input, &i);
			int start = i;
			while (input[i] && (!ft_isspace(input[i]) || in_quote))
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
				else
					i++;
			}
			if (i > start)
				args[arg_idx++] = ft_substr((char *)input, start, i - start);
		}
		else
		{
			int start = i;
			while (input[i] && (!ft_isspace(input[i]) || in_quote))
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
				else
					i++;
			}
			if (i > start)
				args[arg_idx++] = ft_substr((char *)input, start, i - start);
		}
	}
	args[arg_idx] = NULL;
	return args;
}
