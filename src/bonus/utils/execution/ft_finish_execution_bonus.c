/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_finish_execution_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:10:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 16:17:28 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Frees the memory allocated for the command list.
 *
 * SPANISH: Libera la memoria asignada para la lista de comandos.
 *
 * @param cmd_list   Pointer to the head of the command list. /
 *                   Puntero al inicio de la lista de comandos.
 */
static void	ft_free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		ft_free_matrix(cmd_list->argv);
		free(cmd_list);
		cmd_list = tmp;
	}
}

/**
 * ENGLISH: Closes file descriptors for all commands in the list except standard
 * 			input/output.
 *
 * SPANISH: Cierra los descriptores de archivo de todos los comandos en la lista
 * 			excepto la entrada/salida estándar.
 *
 * @param cmd_list   Pointer to the head of the command list. /
 *                   Puntero al inicio de la lista de comandos.
 */
static void	ft_close_fds(t_cmd *cmd_list)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		if (current->infd != STDIN_FILENO)
			close(current->infd);
		if (current->outfd != STDOUT_FILENO)
			close(current->outfd);
		current = current->next;
	}
}

/**
 * ENGLISH: Waits for all child processes to finish and updates the last exit
 * 			status.
 *
 * SPANISH: Espera a que todos los procesos hijos terminen y actualiza el último
 * 			estado de salida.
 *
 * @param pids               Array of process IDs of the child processes. /
 *                           Array de IDs de proceso de los procesos hijos.
 * @param cmd_count         Number of commands (child processes). /
 *                           Número de comandos (procesos hijos).
 * @param last_exit_status  Pointer to store the last exit status. /
 *                           Puntero para almacenar el último estado de salida.
 */
static void	ft_wait_for_children(pid_t *pids, int cmd_count,
		int *last_exit_status)
{
	int	i;
	int	status;
	int	executed_processes;

	executed_processes = 0;
	i = 0;
	while (i < cmd_count)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (WIFEXITED(status))
				*last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*last_exit_status = 128 + WTERMSIG(status);
			executed_processes = 1;
		}
		i++;
	}
	if (!executed_processes)
		*last_exit_status = *last_exit_status;
}

/**
 * ENGLISH: Finalizes the execution of commands by closing file descriptors,
 *          waiting for child processes, updating the last exit status,
 *          freeing allocated memory, and resetting the command list pointer.
 *
 * SPANISH: Finaliza la ejecución de comandos cerrando descriptores de archivo,
 *          esperando a los procesos hijos, actualizando el último estado
 * 			de salida, liberando memoria asignada y reiniciando el puntero
 * 			de la lista de comandos.
 *
 * @param pids       Array of process IDs of the child processes. /
 *                   Array de IDs de proceso de los procesos hijos.
 * @param cmd_list   Pointer to the head of the command list. /
 *                   Puntero al inicio de la lista de comandos.
 * @param data       Pointer to the data structure containing environment
 *                   and status info. /
 *                   Puntero a la estructura de datos que contiene información
 *                   del entorno y estado.
 */
int	ft_finish_execution(pid_t *pids, t_cmd *cmd_list, t_data *data)
{
	int	last_exit_status;

	last_exit_status = data->last_exit_status;
	ft_close_fds(cmd_list);
	ft_wait_for_children(pids, data->cmd_count, &last_exit_status);
	data->last_exit_status = last_exit_status;
	free(pids);
	ft_free_cmd_list(cmd_list);
	cmd_list = NULL;
	return (last_exit_status);
}
