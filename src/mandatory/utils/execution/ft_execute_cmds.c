/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_cmds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:35:40 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/12 17:03:40 by adriescr         ###   ########.fr       */
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

static void	ft_child_exec(t_cmd *curr, t_cmd *cmd_l, t_data **data)
{
	ft_setup_child_io(curr, cmd_l);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (curr->has_error)
		(*data)->last_exit_status = 1;
	else if (ft_is_builtin(curr))
		(*data)->last_exit_status = ft_exec_builtin(curr, data);
	else
		(*data)->last_exit_status = ft_exec_bin(curr);
	exit((*data)->last_exit_status);
}

static int	ft_parent_postfork(pid_t pid, t_cmd *curr, pid_t *pids)
{
	(void)pid;
	pids[curr->index] = pid;
	if (curr->infd != STDIN_FILENO)
		close(curr->infd);
	if (curr->outfd != STDOUT_FILENO)
		close(curr->outfd);
	return (0);
}

static int	ft_fork_exec(t_cmd *curr, t_cmd *cmd_l, t_data **data, pid_t *pids)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		ft_child_exec(curr, cmd_l, data);
	else if (pid > 0)
		return (ft_parent_postfork(pid, curr, pids));
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
		if (ft_fork_exec(current, cmd_list, data, pids) == -1)
		{
			ft_finish_execution(pids, cmd_list, *data);
			return ((*data)->last_exit_status);
		}
		current = current->next;
	}
	ft_finish_execution(pids, cmd_list, *data);
	return ((*data)->last_exit_status);
}
