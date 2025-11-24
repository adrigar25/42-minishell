/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 20:25:36 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/24 15:43:09 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** FUNCION: ft_split
** -----------------
** Divide una cadena en subcadenas utilizando un delimitador dado.
**
** PARAMETROS:
** - char const *s: La cadena a dividir.
** - char c: El delimitador utilizado para dividir la cadena.
**
** RETORNO:
** - Un puntero a un array de cadenas (subcadenas).
** - NULL si la reserva de memoria falla o si la cadena de entrada es NULL.
**
*/

static void	free_result(char **result, size_t j)
{
	size_t	i;

	i = 0;
	while (i < j)
	{
		free(result[i]);
		i++;
	}
	free(result);
}

static char	*split_input(char **result, const char *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			k = i;
			while (s[i] && s[i] != c)
				i++;
			result[j] = ft_substr(s, k, i - k);
			if (!result[j++])
			{
				free_result(result, j - 1);
				return (NULL);
			}
		}
		else
			i++;
	}
	result[j] = NULL;
	return (result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;

	if (!s)
		return (NULL);
	result = (char **)malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!result)
		return (NULL);
	return (split_input(result, s, c));
}
