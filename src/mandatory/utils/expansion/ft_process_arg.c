/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_arg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:59:33 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Processes an argument string, expanding environment variables
 * 		and the special variable '$?', and appending the result to the
 * 		destination string.
 *
 * SPANISH: Procesa una cadena de argumentos, expandiendo variables de
 * 		entorno y la variable especial '$?', y añadiendo el resultado a la
 * 		cadena de destino.
 *
 * @param dst   Pointer to the destination string to append
 * 				the processed argument to. /
 * 				Puntero a la cadena de destino donde se añadirá
 * 				el argumento procesado.
 * @param arg   The argument string to process. /
 * 				La cadena de argumentos a procesar.
 * @param data  Pointer to the shell data structure containing
 * 				environment variables and last exit status. /
 * 				Puntero a la estructura de datos del shell que contiene
 * 				las variables de entorno y el último estado de salida.
 */
int	ft_process_arg(char **dst, char *arg, t_data *data)
{
	int	j;
	int	start;

	j = 0;
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
		return ((*dst = ft_strdup(arg)) != NULL);
	while (arg[j])
	{
		start = j;
		while (arg[j] && arg[j] != '$')
			j++;
		if (j > start && !ft_copy_literal(dst, arg, start, j))
			return (0);
		if (arg[j] == '$')
		{
			if (arg[j + 1] == '?')
			{
				if (!ft_expand_exit_status(dst, &j, data))
					return (0);
			}
			else if (!ft_expand_env_var(dst, arg, &j, data))
				return (0);
		}
	}
	return (1);
}
