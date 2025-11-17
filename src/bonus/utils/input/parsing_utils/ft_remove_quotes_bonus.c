/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_quotes_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:16:59 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/18 00:03:10 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell_bonus.h"

static int	handle_quoted(const char *str, int *i)
{
	int		len;
	char	q;

	len = 0;
	q = str[(*i)++];
	while (str[*i] && str[*i] != q)
	{
		len++;
		(*i)++;
	}
	if (str[*i] == q)
		(*i)++;
	return (len);
}

static int	ft_calc_unquoted_len(const char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && ft_has_closing_quote(str + i
				+ 1, str[i]))
			len += handle_quoted(str, &i);
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

static void	copy_quoted(const char *str, int *i, char *res, int *j)
{
	char	q;

	q = str[(*i)++];
	while (str[*i] && str[*i] != q)
		res[(*j)++] = str[(*i)++];
	if (str[*i] == q)
		(*i)++;
}

static void	copy_char(const char *str, int *i, char *res, int *j)
{
	res[(*j)++] = str[(*i)++];
}

char	*ft_remove_quotes(const char *str)
{
	int		i;
	int		j;
	int		unlen;
	char	*res;

	if (!str)
		return (NULL);
	unlen = ft_calc_unquoted_len(str);
	res = malloc((size_t)unlen + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && ft_has_closing_quote(str + i
				+ 1, str[i]))
			copy_quoted(str, &i, res, &j);
		else
			copy_char(str, &i, res, &j);
	}
	res[j] = '\0';
	return (res);
}
