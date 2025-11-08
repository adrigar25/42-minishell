/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_cmds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:35:40 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/07 15:47:38 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Executes a single built-in command directly if possible.
 *
 * SPANISH: Ejecuta un comando built-in directamente si es posible.
 *
 * @param current   The current command to execute. /
 *                  El comando actual a ejecutar.
 *
 * @param data      Pointer to the shell data structure. /
 *                  Puntero a la estructura de datos del shell.
 *
 * @returns 1 if the built-in was executed directly, 0 otherwise. /
 *          1 si el built-in fue ejecutado directamente, 0 en caso contrario.
 */
static int	ft_execute_single_builtin(t_cmd *current, t_data **data)
{
	if (ft_is_builtin(current) && current->infd == STDIN_FILENO
		&& current->outfd == STDOUT_FILENO && !current->next
		&& current->has_error == 0)
	{
		(*data)->last_exit_status = ft_exec_builtin(current, data);
		return (1);
	}
	return (0);
}

/**
 * ENGLISH: Forks and executes a command in a child process,
 * 			handling built-ins and errors.
 *
 * SPANISH: Hace fork y ejecuta un comando en un proceso hijo,
 * 			gestionando built-ins y errores.
 *
 * @param current   The current command to execute. /
 *                  El comando actual a ejecutar.
 *
 * @param cmd_list  The full list of commands in the pipeline. /
 *                  La lista completa de comandos en la tubería.
 *
 * @param data      Pointer to the shell data structure. /
 *                  Puntero a la estructura de datos del shell.
 *
 * @param pids      Array to store child process IDs. /
 *                  Array para almacenar los IDs de los procesos hijos.
 *
 * @returns 0 on success, -1 on fork error. /
 *          0 en éxito, -1 en error de fork.
 */
static int	ft_fork_and_exec(t_cmd *current, t_cmd *cmd_list, t_data **data,
		pid_t *pids)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		ft_setup_child_io(current, cmd_list);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (current->has_error)
			(*data)->last_exit_status = 1;
		else if (ft_is_builtin(current))
			(*data)->last_exit_status = ft_exec_builtin(current, data);
		else
			(*data)->last_exit_status = ft_exec_bin(current);
		exit((*data)->last_exit_status);
	}
	else if (pid > 0)
	{
		pids[current->index] = pid;
		if (current->infd != STDIN_FILENO)
			close(current->infd);
		if (current->outfd != STDOUT_FILENO)
			close(current->outfd);
		return (0);
	}
	else
		return (perror("fork"), -1);
}

/**
 * ENGLISH: Executes a pipeline of commands, handling built-ins and
 * 			forking as needed.
 *
 * SPANISH: Ejecuta una tubería de comandos, gestionando built-ins y
 * 			haciendo fork según sea necesario.
 *
 * @param cmd_list  The list of commands to execute. /
 *                  La lista de comandos a ejecutar.
 *
 * @param data      Pointer to the shell data structure. /
 *                  Puntero a la estructura de datos del shell.
 *
 * @returns The exit status of the last command executed. /
 *          El estado de salida del último comando ejecutado.
 */
int	ft_execute_cmds(t_cmd *cmd_list, t_data **data)
{
	t_cmd	*current;
	pid_t	*pids;

	if (!cmd_list || !data || !*data)
		return (-1);
	pids = malloc(sizeof(pid_t) * (*data)->cmd_count);
	if (!pids)
		return (-1);
	current = cmd_list;
	while (current)
	{
		if (ft_execute_single_builtin(current, data))
		{
			current = current->next;
			continue ;
		}
		if (ft_fork_and_exec(current, cmd_list, data, pids) == -1)
		{
			ft_finish_execution(pids, cmd_list, *data);
			return ((*data)->last_exit_status);
		}
		current = current->next;
	}
	ft_finish_execution(pids, cmd_list, *data);
	return ((*data)->last_exit_status);
}
