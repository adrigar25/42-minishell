/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 15:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/10 13:35:21 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_result_len(const char *str)
{
	int		len;
	int		i;
	char	quote;
	int		in_quote;

	len = 0;
	i = 0;
	in_quote = 0;
	quote = 0;
	while (str[i])
	{
		if (!in_quote && (str[i] == '\'' || str[i] == '"'))
		{
			in_quote = 1;
			quote = str[i];
		}
		else if (in_quote && str[i] == quote)
		{
			in_quote = 0;
			quote = 0;
		}
		else
		{
			len++;
		}
		i++;
	}
	return (len);
}

char	*ft_remove_quotes(const char *str)
{
	char	*result;
	int		len;
	int		i;
	int		j;
	char	quote;
	int		in_quote;

	if (!str)
		return (NULL);
	len = count_result_len(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_quote = 0;
	quote = 0;
	while (str[i])
	{
		if (!in_quote && (str[i] == '\'' || str[i] == '"'))
		{
			in_quote = 1;
			quote = str[i];
		}
		else if (in_quote && str[i] == quote)
		{
			in_quote = 0;
			quote = 0;
		}
		else
		{
			result[j++] = str[i];
		}
		i++;
	}
	result[j] = '\0';
	return (result);
}
