/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 18:57:27 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:37:17 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Parses the input arguments into a linked list of command structures.
 *          Each command structure contains its arguments, input/output file
 *          descriptors, and logical operators.
 *
 * SPANISH: Analiza los argumentos de entrada en una lista enlazada de
 *          estructuras de comando. Cada estructura de comando contiene
 *          sus argumentos, descriptores de archivo de entrada/salida y
 *          operadores lógicos.
 *
 * @param argv  The array of command arguments. /
 *              El arreglo de argumentos del comando.
 *
 * @param data  Pointer to the shell data structure. /
 *              Puntero a la estructura de datos del shell.
 *
 * @returns A pointer to the head of the linked list of command structures,
 *          or NULL on failure. /
 *          Un puntero a la cabeza de la lista enlazada de estructuras de
 *          comando, o NULL en caso de error.
 */
t_cmd	*ft_parse_input(char **argv, t_data *data)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	int		i;
	int		cmd_index;
	int		new_i;

	if (!argv || data->argc == 0)
		return (NULL);
	cmd_index = 0;
	cmd_list = ft_create_cmd_node(cmd_index);
	if (!cmd_list)
		return (NULL);
	current_cmd = cmd_list;
	current_cmd->data = data;
	i = 0;
	while (i < data->argc)
	{
		i = ft_process_token(&current_cmd, argv, i, &cmd_index);
	}
	return (cmd_list);
}
