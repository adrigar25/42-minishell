/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_cmds_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:32:13 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/18 00:14:51 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

static int	ft_can_exec_builtin_direct(t_cmd *cmd)
{
	return (ft_is_builtin(cmd) && cmd->infd == STDIN_FILENO
		&& cmd->outfd == STDOUT_FILENO && !cmd->next && cmd->has_error == 0);
}

static int	ft_fork_exec(t_cmd *curr, t_cmd *cmd_l, t_data **data, pid_t *pids)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (curr->has_error)
			exit(EXIT_FAILURE);
		ft_setup_child_io(curr, cmd_l);
		if (ft_is_builtin(curr))
			(*data)->last_exit_status = ft_exec_builtin(curr, data);
		else
			(*data)->last_exit_status = ft_exec_bin(curr);
		exit((*data)->last_exit_status);
	}
	else if (pid > 0)
	{
		pids[curr->index] = pid;
		if (curr->infd != STDIN_FILENO)
			close(curr->infd);
		if (curr->outfd != STDOUT_FILENO)
			close(curr->outfd);
		return (SUCCESS);
	}
	return (perror("fork"), ERROR);
}

static void	ft_wait_last(t_data **data, pid_t *pids, int idx)
{
	int	status;

	if (pids[idx] <= 0)
		return ;
	waitpid(pids[idx], &status, 0);
	if (WIFEXITED(status))
		(*data)->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		(*data)->last_exit_status = 128 + WTERMSIG(status);
	pids[idx] = 0;
}

/**
 * ENGLISH: Executes a pipeline of commands, handling built-ins and
 * 			forking as needed. Supports logical operators (&&, ||).
 *
 * SPANISH: Ejecuta una tubería de comandos, gestionando built-ins y
 * 			haciendo fork según sea necesario. Soporta operadores lógicos
 * 			(&&, ||).
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
		return (ERROR);
	pids = calloc((*data)->cmd_count, sizeof(pid_t));
	if (!pids)
		return (ERROR);
	current = cmd_list;
	while (current)
	{
		if (ft_should_execute(&current, *data) == SUCCESS)
		{
			if (ft_can_exec_builtin_direct(current))
				(*data)->last_exit_status = ft_exec_builtin(current, data);
			else if (ft_fork_exec(current, cmd_list, data, pids) == ERROR)
				break ;
			if (!current->next || current->next->op != OP_PIPE)
				ft_wait_last(data, pids, current->index);
		}
		current = current->next;
	}
	ft_finish_execution(pids, cmd_list, *data);
	return ((*data)->last_exit_status);
}
