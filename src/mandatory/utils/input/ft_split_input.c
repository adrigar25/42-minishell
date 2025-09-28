/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 00:33:34 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:55:32 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Checks if a position in the input string is within quotes.
 *
 * SPANISH: Verifica si una posición en la cadena de entrada está dentro
 *          de comillas.
 *
 * @param input The input string to check.
 *               La cadena de entrada a verificar.
 *
 * @param pos   The position to check.
 *               La posición a verificar.
 *
 * @returns 1 if the position is within quotes, 0 otherwise.
 *          1 si la posición está dentro de comillas, 0 en caso contrario.
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
 * ENGLISH: Checks if a character at a given position is escaped by a backslash.
 *
 * SPANISH: Verifica si un carácter en una posición dada está escapado por
 *          una barra invertida.
 *
 * @param input The input string to check.
 *               La cadena de entrada a verificar.
 *
 * @param pos   The position of the character to check.
 *               La posición del carácter a verificar.
 *
 * @param in_q  Flag indicating if currently inside quotes
 *               (1 if inside, 0 if not).
 *               Bandera que indica si actualmente está dentro de comillas
 *               (1 si está dentro, 0 si no).
 *
 * @param q     The quote character if inside quotes (' or "), 0 otherwise.
 *               El carácter de comillas si está dentro de comillas
 *               (' o "), 0 en caso contrario.
 *
 * @returns 1 if the character is escaped, 0 otherwise.
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
 * ENGLISH: Handles token extraction from the input string,
 *          updating the current index and argument list.
 *
 * SPANISH: Maneja la extracción de tokens de la cadena de entrada,
 *          actualizando el índice actual y la lista de argumentos.
 *
 * @param input The input string to parse.
 *               La cadena de entrada a analizar.
 *
 * @param args  The array to store extracted tokens.
 *               El arreglo para almacenar los tokens extraídos.
 *
 * @param i     Pointer to the current index in the input string.
 *               Puntero al índice actual en la cadena de entrada.
 *
 * @param j     Pointer to the current index in the args array.
 *               Puntero al índice actual en el arreglo args.
 *
 * @returns 0 on success, -1 on memory allocation failure.
 *          0 en caso de éxito, -1 en caso de fallo de asignación de memoria.
 */
static int	ft_handle_token(const char *input, char **args, int *i, int *j)
{
	int	start;

	ft_skip_whitespace(input, i);
	if (ft_strchr("<>|", input[*i]) && !ft_is_escaped(input, *i, 0, 0))
	{
		args[*j] = ft_substr((char *)input, *i, 1 + (input[*i
					+ 1] == input[*i]));
		*j += 1;
		*i += 1 + (input[*i + 1] == input[*i]);
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

/**
 * ENGLISH: Splits the input string into tokens based on spaces and special
 *          characters, respecting quotes and escape sequences.
 *
 * SPANISH: Divide la cadena de entrada en tokens basados en espacios y
 *          caracteres especiales, respetando comillas y secuencias de escape.
 *
 * @param input The input string to split.
 *               La cadena de entrada a dividir.
 *
 * @param argc  The number of expected tokens (arguments).
 *               El número de tokens esperados (argumentos).
 *
 * @returns An array of token strings, or NULL on memory allocation failure.
 *          Un arreglo de cadenas de tokens, o NULL en caso de fallo de
 *          asignación de memoria.
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
