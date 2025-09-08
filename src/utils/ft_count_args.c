/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:20:19 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/08 15:55:56 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * ENGLISH: Counts the number of arguments in a command string.
 *
 * SPANISH: Cuenta el número de argumentos en una cadena de comando.
 *
 * @param {const char *} cmd
 *      - The command string to analyze.
 *      - La cadena de comando a analizar.
 *
 * @returns {int}
 *      - Returns the number of arguments found in the command string.
 *      - Retorna el número de argumentos encontrados en la cadena de comando.
 */
int	ft_count_args(const char *cmd)
{
	int	i;
	int	count;
	int	start;

	i = 0;
	count = 0;
	while (cmd[i])
	{
		ft_skip_whitespace(cmd, &i);
		if (!cmd[i])
			break ;
		start = i;
		if (cmd[i] == '\\')
		{
			i += 2;
		}
		else if (cmd[i] == '\'' || cmd[i] == '\"')
		{
			ft_skip_quotes(cmd, &i);
		}
		else if (cmd[i] == '>' || cmd[i] == '<')
		{
			i++;
			if (cmd[i] == '>' || cmd[i] == '<')
				i++;
		}
		else
		{
			while (cmd[i] && !ft_isspace(cmd[i]) && cmd[i] != '\'' && cmd[i] != '\"')
			{
				if (cmd[i] == '\\')
					i += 2;
				else
					i++;
			}
		}
		count++;
	}
	return (count);
}
