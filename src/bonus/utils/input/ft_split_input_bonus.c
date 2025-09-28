/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_input_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:27:22 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:39:03 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Checks if the character at the given position in the input
 * 		string is within quotes.
 *
 *  SPANISH: Verifica si el carácter en la posición dada
 * 		en la cadena de entrada está dentro de comillas.
 *
 * @param input The input string. /
 * 			La cadena de entrada.
 * @param pos   The position to check. /
 * 			La posición a verificar.
 *
 * @returns 1 if the character is within quotes, 0 otherwise. /
 * 		1 si el carácter está dentro de comillas, 0 en caso contrario.
 */
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

/**
 * ENGLISH: Checks if the character at the given position in the input
 *          string is escaped (preceded by an odd number of backslashes).
 *
 * SPANISH: Verifica si el carácter en la posición dada en la cadena de
 *          entrada está escapado (precedido por un número impar de barras
 *          invertidas).
 *
 * @param input   The input string. /
 *                La cadena de entrada.
 *
 * @param pos     The position to check. /
 *                La posición a verificar.
 *
 * @param in_q    Flag indicating if currently inside quotes (1 if inside,
 *                0 otherwise). /
 *                Indicador que indica si actualmente está dentro de comillas
 *                (1 si está dentro, 0 en caso contrario).
 *
 * @param q       The current quote character if inside quotes, 0 otherwise. /
 *                El carácter de comilla actual si está dentro de comillas,
 *                0 en caso contrario.
 *
 * @returns 1 if the character is escaped, 0 otherwise. /
 *          1 si el carácter está escapado, 0 en caso contrario.
 */
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

/**
 * ENGLISH: Handles a single token from the input string,
 *          updating the args array and indices accordingly.
 *
 * SPANISH: Maneja un solo token de la cadena de entrada,
 *          actualizando el arreglo args y los índices en consecuencia.
 *
 * @param input The input string. /
 *              La cadena de entrada.
 *
 * @param args  The array to store the parsed tokens. /
 *              El arreglo para almacenar los tokens analizados.
 *
 * @param i     Pointer to the current index in the input string. /
 *              Puntero al índice actual en la cadena de entrada.
 *
 * @param j     Pointer to the current index in the args array. /
 *              Puntero al índice actual en el arreglo args.
 *
 * @returns 0 on success, -1 on memory allocation failure. /
 *          0 en caso de éxito, -1 en caso de fallo en la asignación de memoria.
 */
static int	ft_handle_token(const char *input, char **args, int *i, int *j)
{
	int	start;

	ft_skip_whitespace(input, i);
	if (ft_strchr("<>|&", input[*i]) && !ft_is_escaped(input, *i, 0, 0))
	{
		args[*j] = ft_substr((char *)input, *i, 1 + (input[*i
					+ 1] == input[*i]));
		*j += 1;
		*i += 1 + (input[*i + 1] == input[*i]);
	}
	else
	{
		start = *i;
		while (input[*i] && (!ft_strchr("<>|&", input[*i])
				|| is_in_quotes(input, *i) || ft_is_escaped(input, *i, 0, 0))
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

/**
 * ENGLISH: Splits the input string into tokens based on spaces and special
 * 			characters, taking into account quotes and escape characters.
 *
 * SPANISH: Divide la cadena de entrada en tokens basados en espacios y
 * 			caracteres especiales, teniendo en cuenta las comillas y
 * 			los caracteres de escape.
 *
 * @param input The input string to split. /
 *              La cadena de entrada a dividir.
 *
 * @param argc  The number of expected tokens. /
 *              El número de tokens esperados.
 *
 * @returns An array of strings (tokens), or NULL on memory allocation failure. /
 *          Un arreglo de cadenas (tokens), o NULL en caso de fallo
 *          en la asignación de memoria.
 */
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
