/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:16:59 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 21:30:30 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Skips a quote character in the string and manages quote state.
 *
 * SPANISH: Omite un carácter de comilla en la cadena
 * y gestiona el estado de comillas.
 *
 * @param str        The input string. /
 *                   La cadena de entrada.
 *
 * @param i          Pointer to the current index in the string. /
 *                   Puntero al índice actual en la cadena.
 *
 * @param quote      Pointer to the current quote character. /
 *                   Puntero al carácter de comilla actual.
 *
 * @param in_quote   Pointer to the quote state (1 if inside quotes,
	0 otherwise). /

	*                   Puntero al estado de comillas
	(1 si está dentro de comillas,0 en caso contrario).
 *
 * @returns 1 if a quote was skipped, 0 otherwise. /
 *          1 si se omitió una comilla, 0 en caso contrario.
 */
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

/**
 * ENGLISH: Calculates the length of the string without quotes.
 *
 * SPANISH: Calcula la longitud de la cadena sin comillas.
 *
 * @param str   The input string. /
 *              La cadena de entrada.
 *
 * @returns The length of the string without quotes. /
 *          La longitud de la cadena sin comillas.
 */
static int	ft_calc_unquoted_len(const char *str)
{
	int		i;
	int		in_q;
	char	q;
	int		len;

	i = -1;
	len = 0;
	in_q = 0;
	q = 0;
	while (str[++i])
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
	}
	return (len);
}

/**

	* ENGLISH: Removes quotes from the input string and
	returns a new string without quotes.
 *

	* SPANISH: Elimina las comillas de la cadena de entrada
	y devuelve una nueva cadena sin comillas.
 *
 * @param str   The input string with quotes. /
 *              La cadena de entrada con comillas.
 *
 * @returns A newly allocated string without quotes,
	or NULL on allocation failure. /
 *          Una cadena recién asignada sin comillas,
	o NULL en caso de fallo en la asignación.
 */
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
