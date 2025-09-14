/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_skip_quotes_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:18:46 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/14 15:18:12 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/*
 * ENGLISH: Skips the current argument in a command string.
 *
 * SPANISH: Omite el argumento actual en una cadena de comando.
 *
 * @param cmd
 *      The command string to process. / La cadena de comando a procesar.
 * @param i
 *      Pointer to the index of the current character in the command string. /
 *      Puntero al índice del carácter actual en la cadena de comando.
 */
void	ft_skip_quotes(const char *cmd, int *i)
{
	int		in_quote;
	char	quote;

	in_quote = 0;
	quote = 0;
	while (cmd[*i])
	{
		if (!in_quote && (cmd[*i] == '\'' || cmd[*i] == '"'))
		{
			in_quote = 1;
			quote = cmd[(*i)++];
		}
		else if (in_quote && cmd[*i] == quote)
		{
			in_quote = 0;
			(*i)++;
		}
		else if (!in_quote && ft_isspace(cmd[*i]))
			break ;
		else
			(*i)++;
	}
}
