/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:20:19 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/08 18:05:29 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_escaped(const char *cmd, int pos, int in_quote, char quote_char)
{
	int	backslashes;
	int	j;

	backslashes = 0;
	j = pos - 1;
	// Solo se chequea fuera de comillas simples
	if (in_quote && quote_char == '\'')
		return (0);
	while (j >= 0 && cmd[j] == '\\')
	{
		backslashes++;
		j--;
	}
	return (backslashes % 2 == 1);
}

/*
 * ENGLISH: Returns the number of arguments parsed from a shell command string.
 *          Handles quoted strings, escaped characters,
	and redirection operators.
 *

	* SPANISH: Devuelve el número de argumentos analizados en una cadena de comando de shell.
 *          Maneja cadenas entre comillas,
	caracteres escapados y operadores de redirección.
 *
 * @param {const char *} cmd
 *      - The command string to parse.
 *      - La cadena de comando a analizar.
 *
 * @returns {int}
 *      - Number of arguments detected in the command string.
 *      - Número de argumentos detectados en la cadena de comando.
 */
int	ft_count_args(const char *cmd)
{
	int		i;
	int		count;
	int		in_quote;
	char	quote_char;
	int		start;

	i = 0;
	count = 0;
	in_quote = 0;
	quote_char = 0;
	while (cmd[i])
	{
		ft_skip_whitespace(cmd, &i);
		if (!cmd[i])
			break ;
		// Handle << or >> as a single argument
		if ((cmd[i] == '<' || cmd[i] == '>') && cmd[i + 1] == cmd[i]
			&& !is_escaped(cmd, i, in_quote, quote_char))
		{
			count++;
			i += 2;
			ft_skip_whitespace(cmd, &i);
			continue ;
		}
		// Handle < or > as a single argument
		else if ((cmd[i] == '<' || cmd[i] == '>') && !is_escaped(cmd, i,
				in_quote, quote_char))
		{
			count++;
			i++;
			ft_skip_whitespace(cmd, &i);
			continue ;
		}
		// Handle pipe as a separate argument, skip consecutive pipes
		else if (cmd[i] == '|' && !is_escaped(cmd, i, in_quote, quote_char))
		{
			count++;
			i++;
			while (cmd[i] == '|' && !is_escaped(cmd, i, in_quote, quote_char))
				i++;
			continue ;
		}
		else
		{
			start = i;
			while (cmd[i] && ((!ft_isspace(cmd[i]) && cmd[i] != '|'
						&& cmd[i] != '<' && cmd[i] != '>') || in_quote))
			{
				if (!in_quote && (cmd[i] == '\'' || cmd[i] == '"'))
				{
					in_quote = 1;
					quote_char = cmd[i++];
				}
				else if (in_quote && cmd[i] == quote_char)
				{
					in_quote = 0;
					quote_char = 0;
					i++;
				}
				else if (cmd[i] == '\\' && cmd[i + 1])
					i += 2;
				else
					i++;
			}
			if (i > start)
				count++;
			// Asegura que i siempre avance al menos una posición si no avanzó
			if (i == start)
				i++;
		}
	}
	return (count);
}
