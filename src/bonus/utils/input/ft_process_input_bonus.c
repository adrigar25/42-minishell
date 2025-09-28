/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_input_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:37:00 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Counts the number of commands in the linked list.
 * 			Used to update the command count in the shell data structure.
 *
 * SPANISH: Cuenta el número de comandos en la lista enlazada.
 * 			Se utiliza para actualizar el recuento de comandos en la estructura
 * 			de datos del shell.
 *
 * @param cmd_list  The head of the command list. /
 *                  La cabeza de la lista de comandos.
 *
 * @returns The number of commands in the list. /
 *          El número de comandos en la lista.
 */
static int	ft_count_cmds(t_cmd *cmd_list)
{
	int		count;
	t_cmd	*head;

	count = 0;
	head = cmd_list;
	while (head && ++count)
		head = head->next;
	return (count);
}

/**
 * ENGLISH: Processes the raw input string into a linked list of command
 *          structures.
 *          It handles splitting, syntax checking, environment variable
 *          expansion, wildcard expansion, and parsing into commands.
 *
 * SPANISH: Procesa la cadena de entrada sin procesar en una lista enlazada de
 *          estructuras de comando. Maneja la división, verificación de sintaxis,
 *          expansión de variables de entorno, expansión de comodines y análisis
 *          en comandos.
 *
 * @param input The raw input string from the user. /
 *              La cadena de entrada sin procesar del usuario.
 *
 * @param data  Pointer to the shell data structure. /
 *              Puntero a la estructura de datos del shell.
 *
 * @param debug Flag to enable debug output. /
 *              Bandera para habilitar la salida de depuración.
 *
 * @returns A pointer to the head of the linked list of command structures,
 *          or NULL on failure. /
 *          Un puntero a la cabeza de la lista enlazada de estructuras de
 *          comando, o NULL en caso de error.
 */
t_cmd	*ft_process_input(char *input, t_data *data, int debug)
{
	t_cmd	*head;
	char	**argv;
	t_cmd	*cmd_list;

	data->argc = ft_count_args(input);
	argv = ft_split_input(input, data->argc);
	free(input);
	if (ft_check_input_syntax(argv, data->argc))
	{
		ft_free_matrix(argv);
		data->last_exit_status = 2;
		return (NULL);
	}
	argv = ft_handle_env_expansion(argv, data);
	argv = ft_handle_wildcards(argv, data);
	cmd_list = ft_parse_input(argv, data);
	if (debug && cmd_list)
		ft_show_debug(argv, data->argc, argv, cmd_list);
	ft_free_matrix(argv);
	data->cmd_count = ft_count_cmds(cmd_list);
	return (cmd_list);
}
