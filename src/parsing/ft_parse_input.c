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
		if (!quote && (str[i] == '\'' || str[i] == '"'))
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

char	**ft_parse_input(const char *input, int argc)
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
			substr = ft_substr((char *)input, i, 1);
			if (!substr)
			{
				args[pos] = NULL;
				return (args);
			}
			args[pos++] = substr;
			i++;
			ft_skip_spaces(input, &i);
			continue ;
		}
		len = 0;
		quote = 0;
		while (input[i])
		{
			if (!quote && ft_isspace(input[i]))
				break ;
			if (!quote && input[i] == '|')
				break ;
			if (!quote && (input[i] == '\'' || input[i] == '"'))
			{
				quote = input[i];
				i++;
				len++;
				continue ;
			}
			else if (quote && input[i] == quote)
			{
				quote = 0;
				i++;
				len++;
				continue ;
			}
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
			{
				args[pos] = NULL;
				return (args);
			}
			substr = ft_trim(substr, ' ');
			tmp = remove_quotes(substr);
			if (!tmp)
			{
				free(substr);
				args[pos] = NULL;
				return (args);
			}
			free(substr);
			args[pos++] = tmp;
		}
	}
	args[pos] = NULL;
	return (args);
}
