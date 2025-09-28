/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_exit_status_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:48:14 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Expands the special variable '$?' in the given argument string,
 *          appending the last exit status to the destination string.
 *
 * SPANISH: Expande la variable especial '$?' en la cadena de argumentos
 * 			dada, añadiendo el último estado de salida a la cadena de
 * 			destino.
 *
 * @param dst   Pointer to the destination string to append the exit
 *              status to. /
 *              Puntero a la cadena de destino donde se añadirá el
 *              estado de salida.
 *
 * @param j     Pointer to the current index in the argument string
 *              (points to '$'). /
 *              Puntero al índice actual en la cadena de argumentos
 *              (apunta a '$').
 *
 * @param data  Pointer to the shell data structure containing the last exit
 *              status. /
 *              Puntero a la estructura de datos del shell que contiene el
 *              último estado de salida.
 *
 * @returns 1 on success, 0 on memory allocation failure. /
 *          1 en caso de éxito, 0 en caso de fallo de asignación de memoria.
 */
int	ft_expand_exit_status(char **dst, int *j, t_data *data)
{
	char	*temp;

	temp = ft_itoa(data->last_exit_status);
	if (!temp)
		return (0);
	if (!ft_append(dst, temp))
	{
		free(temp);
		return (0);
	}
	free(temp);
	(*j) += 2;
	return (1);
}
