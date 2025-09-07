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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int	is_quote_char(char c)
{
	return (c == '\'' || c == '"');
}

static char	**cleanup_and_return(char **args, int pos)
{
	args[pos] = NULL;
	return (args);
}

static int	handle_quote_transition(const char *input, int *i, int *len,
		char *quote)
{
	if (!*quote && is_quote_char(input[*i]))
	{
		*quote = input[*i];
		(*i)++;
		(*len)++;
		return (1);
	}
	else if (*quote && input[*i] == *quote)
	{
		*quote = 0;
		(*i)++;
		(*len)++;
		return (1);
	}
	return (0);
}

static int	should_break_parsing(const char *input, int i, char quote)
{
	if (!quote && ft_isspace(input[i]))
		return (1);
	if (!quote && input[i] == '|')
		return (1);
	return (0);
}

static char	*remove_quotes(const char *str)
{
	char	*res;
	int		i;
	int		j;
	char	quote;

	res = malloc(strlen(str) + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if (!quote && is_quote_char(str[i]))
			quote = str[i];
		else if (quote && str[i] == quote)
			quote = 0;
		else
			res[j++] = str[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

static char	**handle_pipe_token(char **args, int *pos, const char *input,
		int *i)
{
	char	*substr;

	substr = ft_substr((char *)input, *i, 1);
	if (!substr)
		return (cleanup_and_return(args, *pos));
	args[(*pos)++] = substr;
	(*i)++;
	ft_skip_spaces(input, i);
	return (args);
}

char	**ft_split_input(const char *input, int argc)
{
	char	**args;
	int		i;
	int		pos;
	int		start;
	int		len;
	char	*substr;
	char	*tmp;
	char	quote;

	args = malloc((argc + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	pos = 0;
	while (input[i])
	{
		ft_skip_spaces(input, &i);
		if (!input[i])
			break ;
		start = i;
		if (input[i] == '|')
		{
			if (!handle_pipe_token(args, &pos, input, &i))
				return (args);
			continue ;
		}
		len = 0;
		quote = 0;
		while (input[i])
		{
			if (should_break_parsing(input, i, quote))
				break ;
			if (handle_quote_transition(input, &i, &len, &quote))
				continue ;
			i++;
			len++;
		}
		while (len > 0 && ft_isspace(input[start]))
		{
			start++;
			len--;
		}
		if (len > 0)
		{
			substr = ft_substr((char *)input, start, len);
			if (!substr)
				return (cleanup_and_return(args, pos));
			substr = ft_trim(substr, ' ');
			tmp = remove_quotes(substr);
			if (!tmp)
			{
				free(substr);
				return (cleanup_and_return(args, pos));
			}
			free(substr);
			args[pos++] = tmp;
		}
	}
	return (cleanup_and_return(args, pos));
}
