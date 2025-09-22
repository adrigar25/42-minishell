/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_quoted_arg.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:23:56 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/22 13:24:15 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Handles quoted arguments by determining the start and end
 * 			indices for printing.
 *
 * SPANISH: Maneja los argumentos entre comillas determinando los índices
 * 			de inicio y fin para la impresión.
 *
 * @param arg   The argument string to check for quotes. /
 *              La cadena de argumentos a comprobar en busca de comillas.
 *
 * @param start Pointer to store the start index for printing. /
 *              Puntero para almacenar el índice de inicio para la impresión.
 *
 * @param end   Pointer to store the end index for printing. /
 *              Puntero para almacenar el índice de fin para la impresión.
 *
 * @returns 1 if the argument is quoted, 0 otherwise. /
 *          1 si el argumento está entre comillas, 0 en caso contrario.
 */
int	ft_handle_quoted_arg(char *arg, int *start, int *end)
{
	if (ft_is_quoted(arg))
	{
		*start = 1;
		*end = ft_strlen(arg) - 1;
		return (1);
	}
	*start = 0;
	*end = ft_strlen(arg);
	return (0);
}
