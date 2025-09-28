/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 18:57:27 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:54:20 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Parses the input arguments into a linked list of command
 *          structures, handling pipes and redirections.
 *
 * SPANISH: Parsea los argumentos de entrada en una lista enlazada de
 *          estructuras de comando, manejando tuberías y redirecciones.
 *
 * @param argv  The array of input argument strings. /
 *              El arreglo de cadenas de argumentos de entrada.
 *
 * @param data  Pointer to the shell data structure containing
 *              environment variables and other state information. /
 *              Puntero a la estructura de datos del shell que contiene
 *              variables de entorno y otra información de estado.
 *
 * @returns A pointer to the head of the linked list of command
 *          structures, or NULL on failure. /
 *          Un puntero a la cabeza de la lista enlazada de estructuras
 *          de comando, o NULL en caso de error.
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
		new_i = ft_process_token(&current_cmd, argv, i, &cmd_index);
		if (new_i == i)
			i++;
		else
			i = new_i + 1;
	}
	return (cmd_list);
}
