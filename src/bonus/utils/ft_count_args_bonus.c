/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_args_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:20:19 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/28 17:32:55 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell_bonus.h"

/**
 * ENGLISH: Counts the number of arguments in a command string,
 *          considering quotes, escaped characters, and operators.
 *
 * SPANISH: Cuenta el número de argumentos en una cadena de comando,
 *          considerando comillas, caracteres escapados y operadores.
 *
 * @param cmd   The command string to analyze. /
 *              La cadena de comando a analizar.
 *
 * @returns The number of arguments in the command string. /
 *          El número de argumentos en la cadena de comando.
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
 * ENGLISH: Skips over quotes and escaped characters in the command string.
 *
 * SPANISH: Salta las comillas y los caracteres escapados en la cadena de
 * 			comando.
 *
 * @param cmd        The command string to analyze. /
 *                   La cadena de comando a analizar.
 *
 * @param i          The current position in the command string. /
 *                   La posición actual en la cadena de comando.
 *
 * @param in_quote   Indicates if we are inside a quote. /
 *                   Indica si estamos dentro de una comilla.
 *
 * @param quote_char The character of the quote we are in. /
 *                   El carácter de la comilla en la que estamos.
 */
static void	skip_quotes_and_escapes(const char *cmd, int *i, int *in_quote,
		char *quote_char)
{
	while (cmd[*i] && ((*in_quote) || (!ft_isspace(cmd[*i]) && cmd[*i] != '|'
				&& cmd[*i] != '<' && cmd[*i] != '>' && cmd[*i] != '&')))
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
 * ENGLISH: Handles operators in the command string, updating the index
 *          and counting them as arguments.
 *
 * SPANISH: Maneja los operadores en la cadena de comando, actualizando el
 *          índice y contándolos como argumentos.
 *
 * @param cmd        The command string to analyze. /
 *                   La cadena de comando a analizar.
 *
 * @param i          The current position in the command string. /
 *                   La posición actual en la cadena de comando.
 *
 * @param in_quote   Indicates if we are inside a quote. /
 *                   Indica si estamos dentro de una comilla.
 *
 * @param quote_char The character of the quote we are in. /
 *                   El carácter de la comilla en la que estamos.
 *
 * @returns 1 if an operator was handled, 0 otherwise. /
 *          1 si se manejó un operador, 0 en caso contrario.
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
	else if ((cmd[*i] == '|' || cmd[*i] == '&') && !is_escaped(cmd, *i,
			in_quote, quote_char))
	{
		if (cmd[*i + 1] == cmd[*i] && !is_escaped(cmd, *i + 1, in_quote,
				quote_char))
			(*i) += 2;
		else
			(*i)++;
		return (1);
	}
	return (0);
}

/**
 * ENGLISH: Counts a single word in the command string, considering quotes
 *          and escaped characters.
 *
 * SPANISH: Cuenta una sola palabra en la cadena de comando, considerando
 *          comillas y caracteres escapados.
 *
 * @param cmd   The command string to analyze. /
 *              La cadena de comando a analizar.
 *
 * @param i     The current position in the command string. /
 *              La posición actual en la cadena de comando.
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
 * @param cmd   The command string to analyze. /
 *              La cadena de comando a analizar.
 *
 * @returns The number of arguments in the command string. /
 *          El número de argumentos en la cadena de comando.
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
