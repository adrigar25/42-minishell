/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_quotes_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:16:59 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 14:48:37 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

static int	ft_skip_quote(const char *str, int *i, char *quote, int *in_quote)
{
	if (!*in_quote && (str[*i] == '\'' || str[*i] == '"'))
	{
		*in_quote = 1;
		*quote = str[*i];
		return (1);
	}
	else if (*in_quote && str[*i] == *quote)
	{
		*in_quote = 0;
		*quote = 0;
		return (1);
	}
	return (0);
}

static int	ft_calc_unquoted_len(const char *str)
{
	int		i;
	int		in_q;
	char	q;
	int		len;

	i = 0;
	len = 0;
	in_q = 0;
	q = 0;
	while (str[i])
	{
		if ((!in_q && (str[i] == '\'' || str[i] == '"')))
		{
			in_q = 1;
			q = str[i];
		}
		else if (in_q && str[i] == q)
		{
			in_q = 0;
			q = 0;
		}
		else
			len++;
		i++;
	}
	return (len);
}

char	*ft_remove_quotes(const char *str)
{
	int		i;
	char	q;
	char	*res;
	int		j;
	int		in_q;

	i = 0;
	j = 0;
	in_q = 0;
	q = 0;
	if (!str)
		return (NULL);
	res = malloc(ft_calc_unquoted_len(str) + 1);
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (!ft_skip_quote(str, &i, &q, &in_q))
			res[j++] = str[i++];
		else
			i++;
	}
	res[j] = '\0';
	return (res);
}
