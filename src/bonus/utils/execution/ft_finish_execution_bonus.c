/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_finish_execution_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:10:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/17 23:34:18 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

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

static void	ft_handle_signaled(int status, int *last_exit_status)
{
	int	sig;

	sig = WTERMSIG(status);
	*last_exit_status = 128 + sig;
	if (sig == SIGQUIT)
	{
		if (WCOREDUMP(status))
			ft_putstr_fd("Quit (core dumped)\n", 2);
		else
			ft_putstr_fd("Quit\n", 2);
	}
}

static int	ft_check_already_waited(pid_t *pids, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		if (pids[i] == 0)
			return (1);
		i++;
	}
	return (0);
}

static void	ft_wait_for_children(pid_t *pids, int cmd_count,
		int *last_exit_status)
{
	int	i;
	int	status;
	int	already_waited;

	already_waited = ft_check_already_waited(pids, cmd_count);
	i = 0;
	while (i < cmd_count)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (!already_waited)
			{
				if (WIFEXITED(status))
					*last_exit_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					ft_handle_signaled(status, last_exit_status);
			}
		}
		i++;
	}
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
	int		last_exit_status;
	t_cmd	*current;

	last_exit_status = data->last_exit_status;
	ft_close_fds(cmd_list);
	ft_wait_for_children(pids, data->cmd_count, &last_exit_status);
	data->last_exit_status = last_exit_status;
	free(pids);
	current = cmd_list;
	while (current)
	{
		if (current->infd != STDIN_FILENO)
			close(current->infd);
		if (current->outfd != STDOUT_FILENO)
			close(current->outfd);
		current = current->next;
	}
	cmd_list = NULL;
	if (data && data->argv)
	{
		ft_free_matrix(data->argv);
		data->argv = NULL;
	}
	return (last_exit_status);
}
