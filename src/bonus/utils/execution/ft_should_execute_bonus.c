/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_should_execute_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 16:19:59 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:10:31 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Determines if the current command should be executed based on
 *          the previous command's exit status and the logical operator.
 *
 * SPANISH: Determina si el comando actual debe ejecutarse según el estado
 *          de salida del comando anterior y el operador lógico.
 *
 * @param current   Pointer to the current command pointer. /
 *                  Puntero al puntero del comando actual.
 *
 * @param data      Pointer to the shell data structure. /
 *                  Puntero a la estructura de datos del shell.
 *
 * @returns 1 if the command should be skipped, 0 otherwise. /
 *          1 si el comando debe ser omitido, 0 en caso contrario.
 */
int	ft_should_execute(t_cmd **current, t_data *data)
{
	if (!current || !data)
		return (0);
	if (((*current)->op == OP_OR && data->last_exit_status == EXIT_SUCCESS)
		|| ((*current)->op == OP_AND && data->last_exit_status != EXIT_SUCCESS))
	{
		*current = (*current)->next;
		return (1);
	}
	return (0);
}
