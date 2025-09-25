/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 00:33:34 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/25 01:39:26 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_in_quotes(const char *input, int pos)
{
	int		i;
	int		in_q;
	char	q;

	i = 0;
	in_q = 0;
	q = 0;
	while (i <= pos && input[i])
	{
		if (!in_q && (input[i] == '\'' || input[i] == '"'))
		{
			in_q = 1;
			q = input[i];
		}
		else if (in_q && input[i] == q)
		{
			in_q = 0;
			q = 0;
		}
		i++;
	}
	return (in_q);
}

static int	ft_is_escaped(const char *input, int pos, int in_q, char q)
{
	int	count;

	if (in_q)
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

static int	ft_handle_token(const char *input, char **args, int *i, int *j)
{
	int	start;

	ft_skip_whitespace(input, i);
	if (ft_strchr("<>|", input[*i]) && !ft_is_escaped(input, *i, 0, 0))
	{
		args[*j] = ft_substr((char *)input, *i, 1 + (input[*i
					+ 1] == input[*i]));
		*i += 2 + (input[*i + 1] == input[*i]);
	}
	else
	{
		start = *i;
		while (input[*i] && (!ft_strchr("<>|", input[*i]) || is_in_quotes(input,
					*i) || ft_is_escaped(input, *i, 0, 0))
			&& (!ft_isspace(input[*i]) || is_in_quotes(input, *i)))
			*i += 1 + (input[*i] == '\\' && input[*i + 1]);
		if (*i > start)
		{
			args[*j] = ft_substr((char *)input, start, *i - start);
			*j += 1;
		}
	}
	return (0);
}

char	**ft_split_input(const char *input, int argc)
{
	char	**args;
	int		i;
	int		j;

	args = malloc(sizeof(char *) * (argc + 1));
	if (!args)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i] && j < argc)
		ft_handle_token(input, args, &i, &j);
	args[j] = NULL;
	return (args);
}
