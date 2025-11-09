/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_quotes_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:16:59 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/09 14:12:22 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell_bonus.h"

/* Devuelve 1 si en s hay una comilla 'quote' más adelante */
static int	has_closing_quote(const char *s, char quote)
{
	while (*s)
	{
		if (*s == quote)
			return (1);
		s++;
	}
	return (0);
}

/* Calcula la longitud resultante si se eliminan las comillas sintácticas
   (solo se eliminan si existe la comilla de cierre correspondiente). */
static int	ft_calc_unquoted_len(const char *str)
{
	int		i;
	int		len;
	char	q;

	i = 0;
	len = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && has_closing_quote(str + i + 1,
				str[i]))
		{
			q = str[i++];
			while (str[i] && str[i] != q)
			{
				len++;
				i++;
			}
			if (str[i] == q)
				i++;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

/* Elimina las comillas sintácticas (si tienen pareja) y devuelve una nueva
   cadena. Si no hay comilla de cierre, la comilla se mantiene como carácter. */
char	*ft_remove_quotes(const char *str)
{
	int		i;
	int		j;
	int		unlen;
	char	*q;
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
		if ((str[i] == '\'' || str[i] == '"') && has_closing_quote(str + i + 1,
				str[i]))
		{
			q = (char *)&str[i];
			i++;
			while (str[i] && str[i] != *q)
				res[j++] = str[i++];
			if (str[i] == *q)
				i++;
		}
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}
