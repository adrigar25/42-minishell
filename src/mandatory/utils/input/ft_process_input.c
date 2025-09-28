/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 21:35:42 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 18:05:44 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Counts the number of commands in the command list.
 *
 * SPANISH: Cuenta el número de comandos en la lista de comandos.
 *
 * @param cmd_list  Pointer to the head of the command list. /
 *                  Puntero al inicio de la lista de comandos.
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
 * ENGLISH: Processes the input string to create a command list.
 *          This includes counting arguments, splitting input,
 *          checking for syntax errors, handling environment variable expansion,
 *          parsing the input into commands, and optionally showing debug
 * 			information.
 *
 * SPANISH: Procesa la cadena de entrada para crear una lista de comandos.
 *          Esto incluye contar argumentos, dividir la entrada,
 *          comprobar errores de sintaxis, manejar la expansión de variables
 * 			de entorno, analizar la entrada en comandos y opcionalmente mostrar
 * 			información de depuración.
 *
 * @param input   The input command string. /
 *                La cadena de comando de entrada.
 *
 * @param data    Pointer to the data structure containing environment
 * 					and status info. /
 *                Puntero a la estructura de datos que contiene información
 * 					del entorno y estado.
 *
 * @param debug   Flag to indicate if debug information should be shown. /
 *                Indicador para indicar si se debe mostrar información
 * 					de depuración.
 *
 * @returns A pointer to the head of the command list, or NULL on error. /
 *          Un puntero al inicio de la lista de comandos, o NULL
 * 			en caso de error.
 */
t_cmd	*ft_process_input(char *input, t_data *data, int debug)
{
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
	cmd_list = ft_parse_input(argv, data);
	if (debug && cmd_list && argv)
		ft_show_debug(argv, data->argc, argv, cmd_list);
	ft_free_matrix(argv);
	data->cmd_count = ft_count_cmds(cmd_list);
	return (cmd_list);
}
