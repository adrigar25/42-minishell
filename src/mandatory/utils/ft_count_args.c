/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:20:19 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/28 17:57:01 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * ENGLISH: Checks if a character at a given position is escaped by a backslash.
 *
 * SPANISH: Verifica si un carácter en una posición dada está escapado por
 *          una barra invertida.
 *
 * @param cmd       The input command string. /
 *                  La cadena de comando de entrada.
 *
 * @param pos       The position of the character to check. /
 *                  La posición del carácter a verificar.
 *
 * @param in_q      Flag indicating if currently inside quotes
 *                  (1 if inside, 0 if not). /
 *                  Bandera que indica si actualmente está dentro de comillas
 *                  (1 si está dentro, 0 si no).
 *
 * @param quote_char The type of quote currently open (' or "), or 0 if none. /
 *                   El tipo de comilla actualmente abierta (' o "), o 0
 *                   si no hay ninguna.
 *
 * @returns 1 if the character is escaped, 0 otherwise. /
 *          1 si el carácter está escapado, 0 en caso contrario.
 */
int	is_escaped(const char *cmd, int pos, int in_quote, char quote_char)
{
	int	backslashes;
	int	j;

	backslashes = 0;
	j = pos - 1;
	if (in_quote && quote_char == '\'')
		return (0);
	while (j >= 0 && cmd[j] == '\\')
	{
		backslashes++;
		j--;
	}
	return (backslashes % 2 == 1);
}

/**
 * ENGLISH: Skips over quoted sections and escaped characters in the command
 *          string, updating the index accordingly.
 *
 * SPANISH: Omite secciones entre comillas y caracteres escapados en la
 *          cadena de comando, actualizando el índice en consecuencia.
 *
 * @param cmd       The input command string. /
 *                  La cadena de comando de entrada.
 *
 * @param i         Pointer to the current index in the command string. /
 *                  Puntero al índice actual en la cadena de comando.
 *
 * @param in_quote  Pointer to a flag indicating if currently inside quotes
 *                  (1 if inside, 0 if not). /
 *                  Puntero a una bandera que indica si actualmente está
 *                  dentro de comillas (1 si está dentro, 0 si no).
 *
 * @param quote_char Pointer to the type of quote currently open (' or "), or
 *                   0 if none. /
 *                   Puntero al tipo de comilla actualmente abierta (' o "),
 *                   o 0 si no hay ninguna.
 */
static void	skip_quotes_and_escapes(const char *cmd, int *i, int *in_quote,
		char *quote_char)
{
	while (cmd[*i] && ((*in_quote) || (!ft_isspace(cmd[*i]) && cmd[*i] != '|'
				&& cmd[*i] != '<' && cmd[*i] != '>')))
	{
		if (!(*in_quote) && (cmd[*i] == '\'' || cmd[*i] == '"'))
		{
			*in_quote = 1;
			*quote_char = cmd[*i];
			(*i)++;
		}
		else if (*in_quote && cmd[*i] == *quote_char)
		{
			*in_quote = 0;
			*quote_char = 0;
			(*i)++;
		}
		else if (cmd[*i] == '\\' && cmd[*i + 1])
			(*i) += 2;
		else
			(*i)++;
	}
}

/**
 * ENGLISH: Handles detection of operators ('<', '>', '>>', '|') in the
 *          command string, updating the index accordingly.
 *
 * SPANISH: Maneja la detección de operadores ('<', '>', '>>', '|') en la
 *          cadena de comando, actualizando el índice en consecuencia.
 *
 * @param cmd       The input command string. /
 *                  La cadena de comando de entrada.
 *
 * @param i         Pointer to the current index in the command string. /
 *                  Puntero al índice actual en la cadena de comando.
 *
 * @param in_quote  Flag indicating if currently inside quotes
 *                  (1 if inside, 0 if not). /
 *                  Bandera que indica si actualmente está dentro de comillas
 *                  (1 si está dentro, 0 si no).
 *
 * @param quote_char The type of quote currently open (' or "), or 0 if none. /
 *                   El tipo de comilla actualmente abierta (' o "), o 0
 *                   si no hay ninguna.
 *
 * @returns 1 if an operator was found and handled, 0 otherwise. /
 *          1 si se encontró y manejó un operador, 0 en caso contrario.
 */
static int	handle_operator(const char *cmd, int *i, int in_quote,
		char quote_char)
{
	if ((cmd[*i] == '<' || cmd[*i] == '>') && cmd[*i + 1] == cmd[*i]
		&& !is_escaped(cmd, *i, in_quote, quote_char))
	{
		(*i) += 2;
		ft_skip_whitespace(cmd, i);
		return (1);
	}
	else if ((cmd[*i] == '<' || cmd[*i] == '>') && !is_escaped(cmd, *i,
			in_quote, quote_char))
	{
		(*i)++;
		ft_skip_whitespace(cmd, i);
		return (1);
	}
	else if (cmd[*i] == '|' && !is_escaped(cmd, *i, in_quote, quote_char))
	{
		(*i)++;
		while (cmd[*i] == '|' && !is_escaped(cmd, *i, in_quote, quote_char))
			(*i)++;
		return (1);
	}
	return (0);
}

/**
 * ENGLISH: Counts a single word in the command string, considering quotes
 *          and escaped characters, and updates the index accordingly.
 *
 * SPANISH: Cuenta una sola palabra en la cadena de comando, considerando
 *          comillas y caracteres escapados, y actualiza el índice en
 *          consecuencia.
 *
 * @param cmd   The input command string. /
 *              La cadena de comando de entrada.
 *
 * @param i     Pointer to the current index in the command string. /
 *              Puntero al índice actual en la cadena de comando.
 *
 * @returns 1 if a word was counted, 0 otherwise. /
 *          1 si se contó una palabra, 0 en caso contrario.
 */
static int	count_word(const char *cmd, int *i)
{
	int		count;
	int		in_quote;
	char	quote_char;
	int		start;

	count = 0;
	in_quote = 0;
	quote_char = 0;
	start = *i;
	skip_quotes_and_escapes(cmd, i, &in_quote, &quote_char);
	if (*i > start)
		count = 1;
	if (*i == start)
		(*i)++;
	return (count);
}

/**
 * ENGLISH: Counts the number of arguments in a command string,
 *          considering quotes, escaped characters, and operators.
 *
 * SPANISH: Cuenta el número de argumentos en una cadena de comando,
 *          considerando comillas, caracteres escapados y operadores.
 *
 * @param cmd   The input command string. /
 *              La cadena de comando de entrada.
 *
 * @returns The total count of arguments in the command string. /
 *          El conteo total de argumentos en la cadena de comando.
 */
int	ft_count_args(const char *cmd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (cmd[i])
	{
		ft_skip_whitespace(cmd, &i);
		if (!cmd[i])
			break ;
		if (handle_operator(cmd, &i, 0, 0))
			count++;
		else
			count += count_word(cmd, &i);
	}
	return (count);
}
