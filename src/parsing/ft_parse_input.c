/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:58:32 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/05 14:55:18 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char	*remove_quotes(const char *str)
{
	char	*res;
	int		i;
	int		j;
	char	quote;

	res = malloc(strlen(str) + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '"'))
			quote = str[i];
		else if (quote && str[i] == quote)
			quote = 0;
		else
			res[j++] = str[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

/*
 * ENGLISH: Parses the input command string into an array of arguments.
 *
 * SPANISH: Analiza la cadena de comando de entrada en una matriz de argumentos.
 *
 * @param input
 *      The input command string to parse.
	/ La cadena de comando de entrada a analizar.
 * @param argc
 *      The number of arguments expected in the command string. /
 *      El número de argumentos esperados en la cadena de comando.
 * @return
 *      A NULL-terminated array of strings representing the parsed arguments, or
 *      NULL if memory allocation fails. /

	*      Una matriz terminada en NULL de cadenas que representan los argumentos analizados,
	o NULL si la asignación de memoria falla.
 */
char	**ft_parse_input(const char *input, int argc)
{
	char	**args;
	int		i;
	int		j;
	int		pos;
	char	*tmp;

	args = malloc((argc + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	pos = 0;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (!input[i])
			break ;
		j = i;
		ft_skip_quotes(input, &j);
		args[pos] = strndup(&input[i], j - i);
		if (!args[pos])
			return (NULL);
		tmp = remove_quotes(args[pos]);
		free(args[pos]);
		args[pos++] = tmp;
		i = j;
	}
	args[pos] = NULL;
	return (args);
}
